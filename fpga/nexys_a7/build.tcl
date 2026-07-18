set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ../..]]
set build_dir [file join $root_dir build vivado]
set part xc7a100tcsg324-1
set top ZenithSoC
set memory_files [list \
    [file join $root_dir sw examples sd output.hex] \
    [file join $root_dir hw apu synthesis_unit sine sine_quarter.hex] \
]
set project_defines [list \
    _VIVADO_ TRACE BMU FPU \
    BRANCH_PREDICTOR_DEPTH=1024 \
    BRANCH_TARGET_BUFFER_DEPTH=1024 \
    INSTRUCTION_BUFFER_SIZE=8 \
    STORE_BUFFER_DEPTH=8 \
    MUL_PIPE_STAGES=2 \
    "BOOT_START=32'h00000000" \
    "BOOT_END=32'h00003fff" \
    "IO_START=32'h00004000" \
    "IO_END=32'h08003fff" \
    "PRIVATE_REGION_START=32'h00000000" \
    "PRIVATE_REGION_END=32'h7fffffff" \
    "USER_MEMORY_REGION_START=32'h80000000" \
    "USER_MEMORY_REGION_END=32'hffffffff" \
    NON_MASKABLE_INTERRUPT=0 \
    TIMER_INTERRUPT=7 \
    EXTERNAL_INTERRUPT=11 \
    "HARDWARE_RESET_INTERRUPT=32'h7fffffff" \
    INSTR_MISALIGNED=0 \
    INSTR_ACCESS_FAULT=1 \
    INSTR_ILLEGAL=2 \
    BREAKPOINT=3 \
    LOAD_MISALIGNED=4 \
    LOAD_ACCESS_FAULT=5 \
    STORE_MISALIGNED=6 \
    STORE_ACCESS_FAULT=7 \
    U_SYSTEM_CALL=8 \
    M_SYSTEM_CALL=11 \
    SLEEP=16 \
    HANDLER_RETURN=17 \
    STORE_OPERATION=18 \
    LOAD_OPERATION=19 \
    BRANCH_OPERATION=20 \
    JUMP_OPERATION=21 \
    CSR_OPERATION=22 \
    NO_EVENT=31 \
]

set target bitstream
set jobs 4
if {[llength $argv] > 0} {
    set target [lindex $argv 0]
}
if {[llength $argv] > 1} {
    set jobs [lindex $argv 1]
}
if {$target ni {project retry_synth retry_impl finish_routed synth impl bitstream}} {
    error "Unknown target '$target': use project, retry_synth, retry_impl, finish_routed, synth, impl, or bitstream"
}

proc collect_filelist {filelist} {
    set result {}
    set normalized [file normalize $filelist]
    set base [file dirname $normalized]
    set handle [open $normalized r]
    while {[gets $handle line] >= 0} {
        regsub {#.*$} $line {} line
        set line [string trim $line]
        if {$line eq ""} {
            continue
        }
        if {[regexp {^-F\s+(.+)$} $line -> nested]} {
            set result [concat $result [collect_filelist [file join $base [string trim $nested]]]]
        } else {
            lappend result [file normalize [file join $base $line]]
        }
    }
    close $handle
    return $result
}

proc configure_timing_runs {} {
    set_property strategy Flow_PerfOptimized_high [get_runs synth_1]

    set_property strategy Performance_ExplorePostRoutePhysOpt [get_runs impl_1]
    set_property STEPS.PHYS_OPT_DESIGN.ARGS.DIRECTIVE AggressiveExplore [get_runs impl_1]
    set_property STEPS.POST_ROUTE_PHYS_OPT_DESIGN.ARGS.DIRECTIVE AggressiveExplore [get_runs impl_1]
    set_property STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE NoTimingRelaxation [get_runs impl_1]
}

proc write_implementation_reports {build_dir} {
    report_utilization -file [file join $build_dir utilization_impl.rpt]
    report_timing_summary -file [file join $build_dir timing_impl.rpt]
    report_timing -delay_type max -max_paths 500 -nworst 20 -slack_lesser_than 0 \
        -file [file join $build_dir failing_paths_impl.rpt]
    report_bus_skew -file [file join $build_dir bus_skew_impl.rpt]
    report_drc -file [file join $build_dir drc_impl.rpt]
}

if {$target eq "finish_routed"} {
    set routed_dcp [file join $build_dir project ZenithSoC.runs impl_1 ${top}_routed.dcp]
    if {[file exists $routed_dcp]} {
        open_checkpoint $routed_dcp
    } else {
        set physopt_dcp [file join $build_dir project ZenithSoC.runs impl_1 ${top}_physopt.dcp]
        if {![file exists $physopt_dcp]} {
            error "Neither routed nor physical-optimization checkpoint was found"
        }
        open_checkpoint $physopt_dcp
        route_design
        write_checkpoint -force $routed_dcp
    }

    write_implementation_reports $build_dir

    set bit_file [file join $build_dir project ZenithSoC.runs impl_1 ${top}.bit]
    write_bitstream -force $bit_file
    file copy -force $bit_file [file join $build_dir ZenithSoC_nexys_a7.bit]
    puts "Bitstream: [file join $build_dir ZenithSoC_nexys_a7.bit]"
    exit 0
}

if {$target eq "retry_synth"} {
    open_project [file join $build_dir project ZenithSoC.xpr]
    set_property source_mgmt_mode None [current_project]
    configure_timing_runs

    set rtl_files [collect_filelist [file join $root_dir hw _zenithSoC.f]]
    lappend rtl_files [file join $root_dir hw ZenithSoC.sv]
    remove_files [get_files -quiet $rtl_files]
    add_files -norecurse -fileset sources_1 $rtl_files
    add_files -norecurse -fileset sources_1 $memory_files
    set_property file_type {Memory Initialization Files} [get_files $memory_files]

    set header_files {}
    foreach source $rtl_files {
        if {[file extension $source] eq ".svh"} {
            lappend header_files $source
        }
    }
    set_property file_type {Verilog Header} [get_files $header_files]

    set include_dirs {}
    foreach source $rtl_files {
        lappend include_dirs [file dirname $source]
    }
    set_property include_dirs [lsort -unique $include_dirs] [get_filesets sources_1]
    set_property verilog_define $project_defines [get_filesets sources_1]
    set_property top $top [get_filesets sources_1]

    reset_run synth_1
    launch_runs synth_1 -jobs $jobs
    wait_on_run synth_1
    set synth_status [get_property STATUS [get_runs synth_1]]
    if {![string match "*Complete*" $synth_status]} {
        error "Synthesis failed: $synth_status"
    }
    open_run synth_1
    report_utilization -file [file join $build_dir utilization_synth.rpt]
    report_timing_summary -file [file join $build_dir timing_synth.rpt]
    exit 0
}

if {$target eq "retry_impl"} {
    open_project [file join $build_dir project ZenithSoC.xpr]
    configure_timing_runs
    set synth_status [get_property STATUS [get_runs synth_1]]
    if {![string match "*Complete*" $synth_status]} {
        error "Cannot implement an incomplete synthesis run: $synth_status"
    }

    reset_run impl_1
    launch_runs impl_1 -to_step write_bitstream -jobs $jobs
    wait_on_run impl_1
    set impl_status [get_property STATUS [get_runs impl_1]]
    if {![string match "*Complete*" $impl_status]} {
        error "Implementation or bitstream generation failed: $impl_status"
    }

    open_run impl_1
    write_implementation_reports $build_dir

    set bit_file [file join $build_dir project ZenithSoC.runs impl_1 ${top}.bit]
    if {![file exists $bit_file]} {
        error "Bitstream was not created at $bit_file"
    }
    file copy -force $bit_file [file join $build_dir ZenithSoC_nexys_a7.bit]
    puts "Bitstream: [file join $build_dir ZenithSoC_nexys_a7.bit]"
    exit 0
}

file mkdir $build_dir
create_project -force ZenithSoC [file join $build_dir project] -part $part
set_msg_config -id {Common 17-741} -new_severity WARNING
set_property target_language Verilog [current_project]
set_property simulator_language Mixed [current_project]
set_property source_mgmt_mode None [current_project]
configure_timing_runs

set rtl_files [collect_filelist [file join $root_dir hw _zenithSoC.f]]
lappend rtl_files [file join $root_dir hw ZenithSoC.sv]
add_files -norecurse -fileset sources_1 $rtl_files
add_files -norecurse -fileset sources_1 $memory_files
set_property file_type {Memory Initialization Files} [get_files $memory_files]
add_files -norecurse -fileset constrs_1 [file join $root_dir constraint pins.xdc]

set header_files {}
foreach source $rtl_files {
    if {[file extension $source] eq ".svh"} {
        lappend header_files $source
    }
}
set_property file_type {Verilog Header} [get_files $header_files]

set include_dirs {}
foreach source $rtl_files {
    lappend include_dirs [file dirname $source]
}
set include_dirs [lsort -unique $include_dirs]
set_property include_dirs $include_dirs [get_filesets sources_1]
set_property verilog_define $project_defines [get_filesets sources_1]
set_property top $top [get_filesets sources_1]
set_property top_auto_set 0 [get_filesets sources_1]

create_ip -name clk_wiz -vendor xilinx.com -library ip -module_name system_clocking
set_property -dict [list \
    CONFIG.PRIM_IN_FREQ {100.000} \
    CONFIG.PRIM_SOURCE {Single_ended_clock_capable_pin} \
    CONFIG.NUM_OUT_CLKS {2} \
    CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {100.000} \
    CONFIG.CLKOUT2_USED {true} \
    CONFIG.CLKOUT2_REQUESTED_OUT_FREQ {200.000} \
    CONFIG.PRIMARY_PORT {ext_clk_i} \
    CONFIG.CLK_OUT1_PORT {sys_clk_o} \
    CONFIG.CLK_OUT2_PORT {mem_clk_o} \
    CONFIG.USE_RESET {false} \
] [get_ips system_clocking]

create_ip -name mig_7series -vendor xilinx.com -library ip -module_name ddr_controller
set_property -dict [list CONFIG.XML_INPUT_FILE [file join $script_dir mig.prj]] [get_ips ddr_controller]

generate_target all [get_ips {system_clocking ddr_controller}]
export_ip_user_files -of_objects [get_ips {system_clocking ddr_controller}] -no_script -sync -force -quiet
if {$target eq "project"} {
    puts "Project created at [file join $build_dir project ZenithSoC.xpr]"
    exit 0
}

launch_runs synth_1 -jobs $jobs
wait_on_run synth_1
set synth_status [get_property STATUS [get_runs synth_1]]
if {![string match "*Complete*" $synth_status]} {
    error "Synthesis failed: $synth_status"
}
open_run synth_1
report_utilization -file [file join $build_dir utilization_synth.rpt]
report_timing_summary -file [file join $build_dir timing_synth.rpt]

if {$target eq "synth"} {
    exit 0
}

launch_runs impl_1 -to_step route_design -jobs $jobs
wait_on_run impl_1
set impl_status [get_property STATUS [get_runs impl_1]]
if {![string match "*Complete*" $impl_status]} {
    error "Implementation failed: $impl_status"
}
open_run impl_1
write_implementation_reports $build_dir

if {$target eq "impl"} {
    exit 0
}

launch_runs impl_1 -to_step write_bitstream -jobs $jobs
wait_on_run impl_1
set impl_status [get_property STATUS [get_runs impl_1]]
if {![string match "*Complete*" $impl_status]} {
    error "Bitstream generation failed: $impl_status"
}

set bit_file [file join $build_dir project ZenithSoC.runs impl_1 ${top}.bit]
if {![file exists $bit_file]} {
    error "Bitstream was not created at $bit_file"
}
file copy -force $bit_file [file join $build_dir ZenithSoC_nexys_a7.bit]
puts "Bitstream: [file join $build_dir ZenithSoC_nexys_a7.bit]"
