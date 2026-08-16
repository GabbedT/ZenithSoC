#===============================================================================
# postroute_sweep.tcl -- compare post-route phys_opt directives.
#
# This is a short, low-risk experiment: every candidate starts from the same
# routed checkpoint, so the route itself and the 100 MHz constraints are held
# constant.  The candidate checkpoints and reports are written below
# <build_dir>/timing_search/; no candidate is promoted to the deployable
# bitstream automatically.
#
# Usage:
#   vivado -mode batch -source postroute_sweep.tcl \
#       -tclargs <build_dir> [directive ...]
#
# Example:
#   make postroute-sweep POSTROUTE_DIRECTIVES="Explore AggressiveExplore"
#===============================================================================

set script_dir [file dirname [file normalize [info script]]]
set root_dir   [file normalize [file join $script_dir ../..]]

if {[llength $argv] > 0} {
    set build_dir [file normalize [lindex $argv 0]]
} elseif {[info exists ::env(ZENITH_VIVADO_BUILD_DIR)]} {
    set build_dir [file normalize $::env(ZENITH_VIVADO_BUILD_DIR)]
} else {
    set build_dir [file join $root_dir build vivado]
}

set requested [lrange $argv 1 end]
if {[llength $requested] == 0} {
    # Explore is the known best result.  The other two are deliberately kept
    # as explicit experiments rather than hidden in the normal build flow.
    set requested {Explore AggressiveExplore AggressiveFanoutOpt}
}

set impl_dir [file join $build_dir project ZenithSoC.runs impl_1]
set routed_dcp [file join $impl_dir ZenithSoC_routed.dcp]
if {![file exists $routed_dcp]} {
    error "Routed checkpoint not found: $routed_dcp - run make impl first"
}

set output_dir [file join $build_dir timing_search]
file mkdir $output_dir

proc metric_from_current_design {} {
    set paths [get_timing_paths -setup -nworst 1 -max_paths 1]
    if {[llength $paths] == 0} {
        return {clean 0}
    }
    set wns [format %.3f [get_property SLACK [lindex $paths 0]]]
    set failing [llength [get_timing_paths -setup -slack_lesser_than 0.0]]
    return [list $wns $failing]
}

set results {}
foreach directive $requested {
    puts "\n=== post-route directive '$directive' ==="
    set rc [catch {
        catch {close_design}
        open_checkpoint $routed_dcp

        set before [metric_from_current_design]
        puts "  routed baseline: WNS=[lindex $before 0] failing=[lindex $before 1]"

        phys_opt_design -directive $directive

        set after [metric_from_current_design]
        set report_base [file join $output_dir postroute_${directive}]
        report_timing_summary -file ${report_base}_timing_summary.rpt
        report_timing -delay_type max -max_paths 100 -nworst 20 -slack_lesser_than 0 \
            -file ${report_base}_failing_paths.rpt
        write_checkpoint -force ${report_base}.dcp

        lappend results [list $directive [lindex $after 0] [lindex $after 1] PASS]
        puts "  result: WNS=[lindex $after 0] failing=[lindex $after 1]"
        close_design
    } err]
    if {$rc != 0} {
        lappend results [list $directive FAILED - $err]
        puts "  FAILED: $err"
        catch {close_design}
    }
}

puts "\n===================================================================="
puts "POST-ROUTE PHYS_OPT SUMMARY (WNS in ns, negative = violating)"
puts "--------------------------------------------------------------------"
puts [format "%-24s %10s %10s %s" directive WNS(ns) failing status]
foreach result $results {
    puts [format "%-24s %10s %10s %s" [lindex $result 0] [lindex $result 1] \
        [lindex $result 2] [lindex $result 3]]
}
puts "Reports/checkpoints: $output_dir"
puts "===================================================================="
catch {close_design}
exit 0
