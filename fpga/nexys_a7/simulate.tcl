set script_dir [file dirname [file normalize [info script]]]
set root_dir [file normalize [file join $script_dir ../..]]
set build_dir [file join $root_dir build vivado simulation]
source [file join $script_dir project_config.tcl]

set jobs 4
set sim_runtime 1ms
set interactive 0
if {[llength $argv] > 0} {
    set jobs [lindex $argv 0]
}
if {[llength $argv] > 1} {
    set sim_runtime [lindex $argv 1]
}
if {[llength $argv] > 2 && [lindex $argv 2] eq "gui"} {
    set interactive 1
}

if {[lsearch -exact $project_defines VERILATOR] >= 0} {
    error "VERILATOR selects the behavioural DDR replacement and is forbidden in the XSim DDR2 flow"
}
if {[lsearch -exact $project_defines _VIVADO_] < 0} {
    error "_VIVADO_ is required to instantiate the clocking and MIG IP"
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

set rtl_files [collect_filelist [file join $root_dir hw _zenithSoC.f]]
lappend rtl_files [file join $root_dir hw ZenithSoC.sv]

# This is deliberately a curated list.  The VP directory also contains its
# own DUT wrapper and duplicate synchronizer/BRAM implementations, which must
# not be compiled into the full-SoC simulation.
set sd_model_dir [file join $root_dir vp blocks sd rtl]
set sd_model_files [list \
    [file join $sd_model_dir sd_params.vh] \
    [file join $sd_model_dir sd_const.vh] \
    [file join $sd_model_dir synch_3.v] \
    [file join $sd_model_dir sd_bram_block_dp.v] \
    [file join $sd_model_dir sd_wishbone.v] \
    [file join $sd_model_dir sd_mgr.v] \
    [file join $sd_model_dir sd_link.v] \
    [file join $sd_model_dir sd_phy.v] \
    [file join $sd_model_dir sd_top.v] \
    [file join $sd_model_dir bram_whishbone.sv] \
]

set ddr_model_dir [file join $root_dir tb ddr_model]
set ddr_model_files [list \
    [file join $ddr_model_dir ddr2_model_parameters.vh] \
    [file join $ddr_model_dir ddr2_model.v] \
]

set tb_files [concat \
    $sd_model_files \
    $ddr_model_files \
    [list [file join $root_dir tb top SoC_testbench.sv]] \
]

file mkdir $build_dir
create_project -force ZenithSoC_sim [file join $build_dir project] -part $part
set_property target_language Verilog [current_project]
set_property simulator_language Mixed [current_project]
set_property source_mgmt_mode None [current_project]

add_files -norecurse -fileset sources_1 $rtl_files
add_files -norecurse -fileset sources_1 $memory_files
set_property file_type {Memory Initialization Files} [get_files $memory_files]
add_files -norecurse -fileset sim_1 $tb_files

# sd_link.v contains SystemVerilog unsized literals despite its legacy .v
# suffix.  Treat the complete model consistently to avoid language-mode drift.
set sd_systemverilog_files {}
foreach source $sd_model_files {
    if {[file extension $source] in {.v .sv}} {
        lappend sd_systemverilog_files $source
    }
}
set_property file_type SystemVerilog [get_files $sd_systemverilog_files]

set header_files {}
foreach source [concat $rtl_files $tb_files] {
    if {[file extension $source] in {.svh .vh}} {
        lappend header_files $source
    }
}
set_property file_type {Verilog Header} [get_files $header_files]

set include_dirs [list $sd_model_dir $ddr_model_dir]
foreach source $rtl_files {
    lappend include_dirs [file dirname $source]
}
set include_dirs [lsort -unique $include_dirs]
set_property include_dirs $include_dirs [get_filesets sources_1]
set_property include_dirs $include_dirs [get_filesets sim_1]
set_property verilog_define $project_defines [get_filesets sources_1]
set_property verilog_define $project_defines [get_filesets sim_1]
set_property top $top [get_filesets sources_1]
set_property top soc_testbench [get_filesets sim_1]
set_property top_auto_set 0 [get_filesets sources_1]
set_property top_auto_set 0 [get_filesets sim_1]

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
export_ip_user_files -of_objects [get_ips {system_clocking ddr_controller}] \
    -no_script -sync -force -quiet

# Retain internal RTL objects needed by waveform.tcl.  The wave log itself is
# curated, so the generated WDB does not contain a recursive SoC/MIG dump.
set_property xsim.elaborate.debug_level all [get_filesets sim_1]
set_property xsim.simulate.runtime 0ns [get_filesets sim_1]
set_param general.maxThreads $jobs

puts "Starting XSim for $sim_runtime with the MIG and Micron DDR2 model"
launch_simulation -simset sim_1 -mode behavioral
source [file join $script_dir waveform.tcl]
log_selected_waves
if {$interactive} {
    show_selected_waves
    puts "Interactive XSim session ready. Use the Vivado GUI to run, pause, restart, and add signals."
} else {
    run $sim_runtime
    puts "Simulation output: [file join $build_dir project ZenithSoC_sim.sim sim_1 behav xsim]"
    close_sim
    close_project
}
