#===============================================================================
# floorplan_ddr_if.tcl -- floorplan experiment for the DDR load-info crossing.
#
# The DDR timing cluster (WNS -0.372) is a route-dominated dcache->MIG path:
# the dcache (tag/load_cache_controller) sits inside pblock_cpu on the left
# (~X11) while the cache<->MIG bridge (ddr_controller_interface, holding the
# load_info_buffer that is the endpoint of the worst DDR paths) is NOT in any
# pblock and floats mid-die (~X51-56, next to the MIG).  That leaves a ~40
# SLICE-column crossing for the dcache tag/valid -> load_info_buffer paths.
#
# This experiment pins the bridge next to the dcache instead, so the critical
# crossing shrinks.  The AXI path from the bridge out to the MIG lengthens, but
# it has slack to spare.  Run each placement variant and compare WNS.
#
# The extra pblock is added as a constraint file in the constrs_1 fileset so it
# is read by every implementation run; USED_IN_SYNTHESIS is left true for the
# fragment (harmless) but the cells only exist at implementation.
#
# Usage:
#   vivado -mode batch -source floorplan_ddr_if.tcl -tclargs <build_dir> <region>
#
#   <region>   a pblock rectangle, e.g. {SLICE_X20Y20:SLICE_X38Y110}
#   <build_dir> absolute path to the Vivado project parent (default build/vivado)
#===============================================================================

set script_dir [file dirname [file normalize [info script]]]
set root_dir   [file normalize [file join $script_dir ../..]]

if {[llength $argv] > 0} {
    set region_arg [lindex $argv 0]
} else {
    set region_arg {SLICE_X20Y20:SLICE_X38Y110}
}
if {[llength $argv] > 1} {
    set build_dir [file normalize [lindex $argv 1]]
} else {
    set build_dir [file join $root_dir build vivado]
}

set proj_dir [file join $build_dir project ZenithSoC.xpr]
if {![file exists $proj_dir]} {
    error "Project not found at $proj_dir - run 'make project' first"
}

set run  impl_1
set jobs 8

# --- write the fragment ------------------------------------------------------
set frag [file join $script_dir floorplan_ddr_if_frag.xdc]
set fh [open $frag w]
puts $fh "# Experiment: pin ddr_controller_interface next to the dcache."
puts $fh "create_pblock pblock_ddr_if"
puts $fh "add_cells_to_pblock \[get_pblocks pblock_ddr_if\] \[get_cells ddr_controller_interface\]"
puts $fh "resize_pblock \[get_pblocks pblock_ddr_if\] -add {$region_arg}"
puts $fh "# keep MIG/IOB/BUFG out"
puts $fh "remove_cells_from_pblock \[get_pblocks pblock_ddr_if\] \[get_cells -hier -filter {PRIMITIVE_TYPE =~ IOB.* || PRIMITIVE_TYPE =~ BUFG.* || PRIMITIVE_TYPE =~ OTHERS.others.*}\]"
close $fh
puts "Wrote fragment: $frag"
puts "Region: $region_arg"

open_project $proj_dir
set_property source_mgmt_mode None [current_project]

# Apply the existing strategy/directives (from build.tcl).
set_property strategy Performance_NetDelay_high [get_runs $run]
set_property STEPS.PLACE_DESIGN.ARGS.DIRECTIVE ExtraNetDelay_high [get_runs $run]
set_property STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE AggressiveExplore [get_runs $run]
set_property STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE NoTimingRelaxation [get_runs $run]
set_property STEPS.POST_ROUTE_PHYS_OPT_DESIGN.IS_ENABLED true [get_runs $run]
set_property STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE AggressiveExplore [get_runs $run]

# Add the fragment to the constraint fileset.
set fileset constrs_1
set in_fs [get_files -of_objects [get_filesets $fileset] $frag]
if {[llength $in_fs] == 0} {
    add_files -norecurse -fileset $fileset $frag
    puts "Added $frag to $fileset"
} else {
    puts "Fragment already in $fileset"
}

puts "Launching implementation with ddr_controller_interface pinned to $region_arg"
reset_run $run
launch_runs $run -jobs $jobs
wait_on_run $run
set status [get_property STATUS [get_runs $run]]
puts "impl status: $status"
if {![string match "*Complete*" $status]} {
    puts "Run did not complete: $status"
    exit 1
}

open_run $run
set paths [get_timing_paths -setup -nworst 1 -max_paths 1 -slack_lesser_than 0.0]
if {[llength $paths] > 0} {
    set wns [format %.3f [get_property SLACK [lindex $paths 0]]]
} else {
    set wns "clean"
}
set nfail [llength [get_timing_paths -setup -slack_lesser_than 0.0]]
puts "DDR-IF FLOORPLAN WNS=$wns failing_endpoints=$nfail (region=$region_arg)"
report_timing_summary -file [file join $build_dir timing_ddr_if_floorplan.rpt]
exit 0
