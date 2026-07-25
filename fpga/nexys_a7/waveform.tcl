# Curated XSim waveform set for the Nexys A7 full-SoC simulation.
# This file is sourced after launch_simulation, so all paths are XSim paths.

proc selected_wave_objects {path} {
    set objects [get_objects -quiet $path]
    if {[llength $objects] == 0} {
        puts "WARNING: waveform object not found: $path"
        return {}
    }
    return $objects
}

proc selected_waveform_objects {} {
    set dut /soc_testbench/dut
    set objects {}

    # Clocking and reset sequencing.
    foreach path [list \
        "$dut/sys_clk" \
        "$dut/ddr_clk" \
        "$dut/locked" \
        "$dut/ddr_ready" \
        "$dut/reset_n" \
        "$dut/rst_n_i" \
        "$dut/ddr_reset_n" \
    ] {
        set objects [concat $objects [selected_wave_objects $path]]
    }

    # XSim may flatten part of the CPU hierarchy in a static WDB.  Resolve
    # these internal objects recursively instead of relying on a source-level
    # instance path.
    set pc_objects [get_objects -quiet -r *program_counter*]
    if {[llength $pc_objects] == 0} {
        puts "WARNING: ApogeoRV program-counter waveform objects not found"
    } else {
        set objects [concat $objects $pc_objects]
    }

    # UART MMIO, control/status, and TX/RX state.  This wildcard also handles
    # more than one UART instance.
    set uart_objects [get_objects -quiet -r *uart*]
    if {[llength $uart_objects] == 0} {
        puts "WARNING: UART waveform objects not found"
    } else {
        set objects [concat $objects $uart_objects]
    }

    return $objects
}

proc log_selected_waves {} {
    set objects [selected_waveform_objects]
    if {[llength $objects] > 0} {
        log_wave $objects
    }
}

proc show_selected_waves {} {
    if {[string length [current_wave_config]] == 0} {
        create_wave_config
    }
    set objects [selected_waveform_objects]
    if {[llength $objects] > 0} {
        add_wave $objects
    }
}
