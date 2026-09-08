#===============================================================================
# sweep_impl.tcl -- implementation strategy/directive sweep.
#
# Reuses the already-synthesized netlist (synth_1) and runs a set of
# implementation configurations, each resetting impl_1 with its own strategy
# and per-step directives.  For every configuration it records the worst
# negative slack (WNS), total negative slack (TNS) and the number of failing
# setup endpoints, then prints a comparison table.
#
# Every optional directive is reset before a candidate is launched.  This is
# important when a strategy is changed on an existing project: an explicit
# directive left by the previous candidate otherwise silently contaminates
# the next result.
#
# No placement constraints are added here.  Every candidate uses the same
# synthesized netlist and the project's existing non-floorplanning constraints,
# so differences come only from implementation strategy/directive choices.
#
# Usage:
#   vivado -mode batch -source sweep_impl.tcl -tclargs <build_dir> [cfgA cfgB ...]
#
#   <build_dir>   absolute path to the Vivado project's parent dir
#                 (defaults to ../../build/vivado, i.e. the standard flow)
#   [cfgA ...]    names of configurations to run (must be keys of CONFIGS).
#                 With no names, every configuration is run.
#
# Each configuration is a dict:
#   strategy             implementation strategy
#   place_directive      STEPS.PLACE_DESIGN.ARGS.DIRECTIVE ("" = strategy default)
#   phys_opt_directive   STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE ("" = default)
#   route_directive      STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE ("" = default)
#   post_phys_opt        whether to run post-route physical optimization
#   post_phys_directive  STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE
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

set proj_dir [file join $build_dir project ZenithSoC.xpr]
if {![file exists $proj_dir]} {
    error "Project not found at $proj_dir - run 'make project' first"
}

# --- configuration table -----------------------------------------------------
#   name -> {strategy place phys route post post_dir}
# Every entry enables post-route physical optimization (the proven help for
# these route-dominated LSU/commit paths) unless noted otherwise.
set CONFIGS {
    baseline \
        {Performance_NetDelay_high ExtraNetDelay_high AggressiveExplore NoTimingRelaxation true AggressiveExplore}
    explore_postroute_physopt \
        {Performance_ExplorePostRoutePhysOpt "" "" "" true ""}
    explore \
        {Performance_Explore "" "" "" true ""}
    refine_placement \
        {Performance_RefinePlacement "" "" "" true ""}
    netdelay_low \
        {Performance_NetDelay_low ExtraNetDelay_low AggressiveExplore NoTimingRelaxation true AggressiveExplore}
    netdelay_high_route_explore \
        {Performance_NetDelay_high ExtraNetDelay_high AggressiveExplore Explore true AggressiveExplore}
}

proc cfg_value {cfg idx} {
    return [lindex $cfg $idx]
}

proc reset_run_prop {run prop} {
    # reset_property restores the strategy default.  Keep this in a helper so
    # empty entries in CONFIGS cannot inherit a previous candidate's setting.
    catch {reset_property $prop [get_runs $run]}
}

proc set_optional_run_prop {run prop value} {
    if {$value eq ""} {
        reset_run_prop $run $prop
    } else {
        set_property $prop $value [get_runs $run]
    }
}

# --- helpers ----------------------------------------------------------------
proc apply_config {run cfg} {
    set strategy [cfg_value $cfg 0]
    set place    [cfg_value $cfg 1]
    set phys     [cfg_value $cfg 2]
    set route    [cfg_value $cfg 3]
    set post     [cfg_value $cfg 4]
    set postdir  [cfg_value $cfg 5]

    set_property strategy $strategy [get_runs $run]
    set_optional_run_prop $run STEPS.PLACE_DESIGN.ARGS.DIRECTIVE $place
    set_optional_run_prop $run STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE $phys
    set_optional_run_prop $run STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE $route
    set_property STEPS.POST_ROUTE_PHYS_OPT_DESIGN.IS_ENABLED $post [get_runs $run]
    set_optional_run_prop $run STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE $postdir
    puts "  [clock format [clock seconds] -format %T] strategy=$strategy place=$place phys=$phys route=$route post=$post/$postdir"
}

proc timing_metrics {report_file} {
    # Parse Vivado's authoritative design-summary row.  This avoids relying on
    # collection iteration commands that differ between Vivado Tcl versions.
    set handle [open $report_file r]
    set found_header false
    set metrics {}
    while {[gets $handle line] >= 0} {
        if {!$found_header} {
            if {[regexp {WNS\(ns\).*TNS\(ns\).*TNS Failing Endpoints} $line]} {
                set found_header true
            }
            continue
        }

        if {[regexp {^[[:space:]]*(-?[0-9]+[.][0-9]+)[[:space:]]+(-?[0-9]+[.][0-9]+)[[:space:]]+([0-9]+)[[:space:]]+} \
                $line -> wns tns failing]} {
            set metrics [list $wns $tns $failing]
            break
        }
    }
    close $handle

    if {[llength $metrics] != 3} {
        error "could not parse timing summary from $report_file"
    }
    return $metrics
}

# --- main -------------------------------------------------------------------
open_project $proj_dir
set_property source_mgmt_mode None [current_project]

set run impl_1
set jobs 8
if {[info exists ::env(SWEEP_JOBS)] && $::env(SWEEP_JOBS) ne ""} {
    set jobs $::env(SWEEP_JOBS)
}

if {[info exists ::env(SWEEP_DRY_RUN)]} {
    puts "DRY RUN: opening project only, no implementation will be launched"
    foreach {n c} $CONFIGS {
        puts [format "  %-28s strategy=%-32s place=%-16s phys=%-16s route=%-16s post=%s" \
            $n [cfg_value $c 0] [cfg_value $c 1] [cfg_value $c 2] [cfg_value $c 3] [cfg_value $c 4]]
    }
    exit 0
}

# Names to sweep (default: all)
set names {}
if {[llength $requested] > 0} {
    foreach n $requested { set names [lappend names $n] }
} else {
    foreach {n c} $CONFIGS { set names [lappend names $n] }
}

set output_dir [file join $build_dir timing_search impl_sweep]
file mkdir $output_dir
set summary_file [file join $output_dir summary.tsv]
set summary_handle [open $summary_file w]
puts $summary_handle "config\tWNS(ns)\tTNS(ns)\tfailing\tstatus"
flush $summary_handle

set results {}
puts "Sweep output directory: $output_dir"

foreach name $names {
    # locate the config dict
    set cfg ""
    foreach {n c} $CONFIGS {
        if {$n eq $name} { set cfg $c }
    }
    if {$cfg eq ""} {
        puts "WARNING: unknown configuration '$name', skipping"
        continue
    }
    puts "\n=== [clock format [clock seconds] -format %T] running config '$name' ==="
    set rc [catch {
        reset_runs $run
        apply_config $run $cfg
        launch_runs $run -jobs $jobs
        wait_on_run $run
        set status [get_property STATUS [get_runs $run]]
        if {![string match "*Complete*" $status]} {
            error "implementation did not complete: $status"
        }
        open_run $run
        set report_base [file join $output_dir $name]
        report_timing_summary -file ${report_base}_timing_summary.rpt
        report_timing -delay_type max -max_paths 50 -nworst 10 -slack_lesser_than 0 \
            -file ${report_base}_failing_paths.rpt
        report_utilization -file ${report_base}_utilization.rpt
        write_checkpoint -force ${report_base}.dcp

        # Save artifacts before calculating the compact summary.  A reporting
        # helper failure must never discard a completed implementation.
        set metrics [timing_metrics ${report_base}_timing_summary.rpt]
        set wns [lindex $metrics 0]
        set tns [lindex $metrics 1]
        set nfail [lindex $metrics 2]

        lappend results [list $name $wns $tns $nfail $status]
        puts $summary_handle "$name\t$wns\t$tns\t$nfail\t$status"
        flush $summary_handle
        puts "  config '$name': WNS=$wns  TNS=$tns  failing_endpoints=$nfail"
        close_design
    } err opts]
    if {$rc != 0} {
        lappend results [list $name FAILED - - $err]
        puts $summary_handle "$name\tFAILED\t-\t-\t$err"
        flush $summary_handle
        puts "  config '$name' failed: $err"
        catch {close_design}
    }
}

puts "\n===================================================================="
puts "SWEEP SUMMARY (slack in ns, negative = violating)"
puts "--------------------------------------------------------------------"
puts [format "%-28s %10s %12s %10s" "config" "WNS(ns)" "TNS(ns)" "failing"]
foreach r $results {
    puts [format "%-28s %10s %12s %10s" [lindex $r 0] [lindex $r 1] \
        [lindex $r 2] [lindex $r 3]]
}
close $summary_handle
puts "Reports, checkpoints and summary: $output_dir"
puts "===================================================================="

# impl_1 contains the last candidate, while every candidate artifact remains
# under timing_search.  A later retry-impl reapplies build.tcl's default
# configuration; no sweep result is promoted implicitly.
catch {close_design}
exit 0
