#===============================================================================
# physopt_forced_repl.tcl -- targeted driver replication for high-fanout nets.
#
# phys_opt_design's normal directives replicate net drivers only when the
# replication is safe and beneficial; some high-fanout nets (the ones that
# dominate the route delay of the LSU/commit/predictor/DDR paths) are skipped.
# Vivado explicitly suggests forcing them: `-force_replication_on_nets`.
# Because that option is exclusive of every other phys_opt_design option, it
# must run as its own pass:
#
#   phys_opt_design -directive <pre>     (normal, pre-route)
#   phys_opt_design -force_replication_on_nets {...} (forced, pre-route)
#   route_design
#   phys_opt_design -directive <post>    (post-route)
#
# This runs the implementation steps MANUALLY on the open synthesis netlist in
# a single session (no launch_runs / intermediate-checkpoint reopening, which
# would break on the run's incremental-placement sidecar files).  The project's
# constraint filesets (pins.xdc, floorplan.xdc, timing XDC) apply to the open
# run automatically.
#
# Usage:
#   vivado -mode batch -source physopt_forced_repl.tcl -tclargs <build_dir> <net1> <net2> ...
#
#   <build_dir>  absolute path to the Vivado project parent (default build/vivado)
#   <netN>       hierarchical net names to force replication on. A "*" runs the
#                pass for every net with TOTAL_FANOUT >= 25 (best-effort).
#                Pass "@path/to/file" to read the list from a file (one net per
#                line, # comments and blank lines ignored).
#
# Implementation configuration: defaults to build.tcl's
# Performance_ExplorePostRoutePhysOpt; use REPL_CONFIG=<sweep config> to stack the forced
# pass on a sweep_impl.tcl configuration, or set the step directives directly:
#   REPL_CONFIG     name of a sweep_impl.tcl CONFIGS entry (applied first)
#   REPL_STRATEGY   impl_1 strategy (only used as a fallback default)
#   REPL_PLACE      PLACE_DESIGN directive (default "Explore")
#   REPL_PHYSOPT    PHYS_OPT_DESIGN directive (default "Explore")
#   REPL_ROUTE      ROUTE_DESIGN directive (default "Explore")
#   REPL_POSTDIR    post-route physopt directive (default "Explore")
#===============================================================================

set script_dir [file dirname [file normalize [info script]]]
set root_dir   [file normalize [file join $script_dir ../..]]

if {[llength $argv] > 0} {
    set build_dir [file normalize [lindex $argv 0]]
} else {
    set build_dir [file join $root_dir build vivado]
}
set net_args [lrange $argv 1 end]

set proj_dir [file join $build_dir project ZenithSoC.xpr]
if {![file exists $proj_dir]} {
    error "Project not found at $proj_dir - run 'make project' first"
}

# --- sweep configuration table (must match sweep_impl.tcl) -------------------
#   name -> {strategy place phys route post post_dir}
set SWEEP_CONFIGS {
    baseline \
        {Performance_ExplorePostRoutePhysOpt Explore Explore Explore true Explore}
    explore_postroute_physopt \
        {Performance_ExplorePostRoutePhysOpt Explore Explore Explore true Explore}
    explore \
        {Performance_Explore Explore Explore Explore true Explore}
    refine_placement \
        {Performance_RefinePlacement Explore Explore Explore true Explore}
    netdelay_low \
        {Performance_NetDelay_low ExtraNetDelay_low AggressiveExplore NoTimingRelaxation true AggressiveExplore}
    netdelay_high_route_explore \
        {Performance_NetDelay_high ExtraNetDelay_high AggressiveExplore Explore true AggressiveExplore}
}

# --- resolve step directives ------------------------------------------------
# Precedence: explicit REPL_* env > REPL_CONFIG entry > defaults below.
set place_dirv Explore
set phys_dirv  Explore
set route_dirv Explore
set post_dirv  Explore

if {[info exists ::env(REPL_CONFIG)]} {
    set cfg ""
    foreach {cname cval} $SWEEP_CONFIGS {
        if {$cname eq $::env(REPL_CONFIG)} { set cfg $cval }
    }
    if {$cfg eq ""} { error "unknown REPL_CONFIG '$::env(REPL_CONFIG)'" }
    set place_dirv [lindex $cfg 1]
    set phys_dirv  [lindex $cfg 2]
    set route_dirv [lindex $cfg 3]
    set post_dirv  [lindex $cfg 5]
    puts "Using sweep config '$::env(REPL_CONFIG)': place=$place_dirv phys=$phys_dirv route=$route_dirv post=$post_dirv"
}
if {[info exists ::env(REPL_PLACE)]}   { set place_dirv $::env(REPL_PLACE) }
if {[info exists ::env(REPL_PHYSOPT)]} { set phys_dirv  $::env(REPL_PHYSOPT) }
if {[info exists ::env(REPL_ROUTE)]}   { set route_dirv $::env(REPL_ROUTE) }
if {[info exists ::env(REPL_POSTDIR)]} { set post_dirv  $::env(REPL_POSTDIR) }

open_project $proj_dir
set_property source_mgmt_mode None [current_project]

puts "Step directives: place=$place_dirv phys=$phys_dirv route=$route_dirv post=$post_dirv"

# --- open a design at the pre-route physopt stage -----------------------------
# Resume from the forced physopt checkpoint if it exists (written by a previous
# run of this script); otherwise redo opt+place+physopt on the synthesis
# netlist.  These checkpoints are written with write_checkpoint, so they are
# self-contained (no incremental-placement sidecar files).
set impl_dir [file join $build_dir project ZenithSoC.runs impl_1]
set physopt_dcp [file join $impl_dir ZenithSoC_forced_physopt.dcp]
if {[file exists $physopt_dcp]} {
    open_checkpoint $physopt_dcp
    puts "Resumed from $physopt_dcp"
} else {
    set synth_status [get_property STATUS [get_runs synth_1]]
    if {![string match "*Complete*" $synth_status]} {
        error "synth_1 is '$synth_status' - synthesize first (make synth)"
    }
    open_run synth_1
    puts "Opened synthesis netlist"

    # --- sanity-check that the run's constraints were applied -----------------
    set clks [get_clocks -quiet]
    puts "Design clocks: [llength $clks]"
    if {[llength $clks] == 0} {
        error "No clocks in open design - run constraints not applied; aborting to avoid an unconstrained result"
    }
    set pblocks [get_pblocks -quiet -filter {NAME =~ pblock_*}]
    set n_cpu_mig 0
    foreach pb $pblocks {
        if {[string match "*pblock_cpu" $pb] || [string match "*pblock_mig" $pb]} { incr n_cpu_mig }
    }
    puts "Floorplan pblocks found: $n_cpu_mig/2 ($pblocks)"

    opt_design
    write_checkpoint -force [file join $impl_dir ZenithSoC_forced_opt.dcp]

    if {$place_dirv eq ""} {
        place_design
    } else {
        place_design -directive $place_dirv
    }
    write_checkpoint -force [file join $impl_dir ZenithSoC_forced_placed.dcp]

    if {$phys_dirv eq ""} {
        phys_opt_design
    } else {
        phys_opt_design -directive $phys_dirv
    }
    write_checkpoint -force $physopt_dcp
}

# --- resolve the net list ----------------------------------------------------
# Priority: an explicit list (via -tclargs or @file) is used as-is; otherwise,
# or if the explicit list does not resolve (names may come from an older run),
# nets are derived from the worst failing setup paths in the CURRENT design.
#
# Lookup gotchas discovered empirically on this design:
#   * get_nets -hier <fullpath>          returns 0 for hierarchical names
#   * get_nets -filter {NAME eq "..."}   rejects brackets inside the string
#   * get_nets -hier -regexp ".*...$"    works once [ ] are regex-escaped
#   * TOTAL_FANOUT / FANOUT properties   are empty on post-physopt nets; the
#     fanout proxy that works is PIN_COUNT (or FLAT_PIN_COUNT).
set raw_args {}
foreach a $net_args {
    if {[string index $a 0] eq "@"} {
        set fpath [string range $a 1 end]
        if {![file exists $fpath]} { error "net list file not found: $fpath" }
        set fh [open $fpath r]
        while {[gets $fh line] >= 0} {
            set line [string trim $line]
            if {$line eq "" || [string index $line 0] eq "#"} { continue }
            lappend raw_args $line
        }
        close $fh
        puts "Read [llength $raw_args] net names from $fpath"
    } else {
        lappend raw_args $a
    }
}
set net_args $raw_args

set nets {}
if {[llength $net_args] > 0 && [lindex $net_args 0] eq "*"} {
    # 'All nets with PIN_COUNT >= 25' (fanout proxy).  Also captures nets that
    # do not sit on a failing path (harmless - replication is only a cost).
    set nets [get_nets -hier -quiet -filter {PIN_COUNT >= 25}]
    puts "Forced replication on [llength $nets] nets with PIN_COUNT >= 25 (explicit '*')"
} else {
    foreach n $net_args {
        set esc [string map {[ {\[} ] {\]} . {\.}} $n]
        set obj [get_nets -hier -quiet -regexp ".*${esc}$"]
        if {[llength $obj] == 0} {
            puts "WARNING: net '$n' not found"
        } else {
            lappend nets $obj
        }
    }
}
if {[llength $net_args] > 0 && [llength $nets] == 0 && [lindex $net_args 0] ne "*"} {
    puts "Explicit net list resolved to nothing; deriving nets from failing timing paths instead"
}
if {[llength $nets] == 0} {
    # Derive from the worst failing setup paths in the open (post-physopt) design:
    # nets on those paths with PIN_COUNT >= 20 (fanout proxy), deduped.
    set paths [get_timing_paths -setup -max_paths 300 -slack_lesser_than 0.0]
    puts "Deriving nets from [llength $paths] failing setup paths"
    set pinct {}
    foreach p $paths {
        lappend pinct [get_pins -quiet -of_objects $p]
    }
    set allpins [lsort -unique [concat $pinct]]
    set candidates [get_nets -quiet -of_objects $allpins]
    set nets {}
    foreach c $candidates {
        if {[get_property PIN_COUNT $c] >= 20} {
            lappend nets $c
        }
    }
    set nets [lsort -unique $nets]
}
if {[llength $nets] == 0} {
    error "No nets selected for forced replication"
}
puts "Forced replication on [llength $nets] nets"

# --- extra pass: forced driver replication -----------------------------------
phys_opt_design -force_replication_on_nets $nets
write_checkpoint -force [file join $build_dir project ZenithSoC.runs impl_1 ZenithSoC_forced_repl.dcp]

if {$route_dirv eq ""} {
    route_design
} else {
    route_design -directive $route_dirv
}
write_checkpoint -force [file join $build_dir project ZenithSoC.runs impl_1 ZenithSoC_forced_routed.dcp]

if {$post_dirv eq ""} {
    phys_opt_design
} else {
    phys_opt_design -directive $post_dirv
}
write_checkpoint -force [file join $build_dir project ZenithSoC.runs impl_1 ZenithSoC_forced_post.dcp]

# --- reports ----------------------------------------------------------------
report_timing_summary -file [file join $build_dir timing_forced_repl.rpt]
report_timing -delay_type max -max_paths 500 -nworst 20 -slack_lesser_than 0 \
    -file [file join $build_dir failing_paths_forced_repl.rpt]

set paths [get_timing_paths -setup -nworst 1 -max_paths 1 -slack_lesser_than 0.0]
if {[llength $paths] > 0} {
    puts "FORCED REPL WNS: [format %.3f [get_property SLACK [lindex $paths 0]]]"
} else {
    puts "FORCED REPL WNS: clean"
}
exit 0
