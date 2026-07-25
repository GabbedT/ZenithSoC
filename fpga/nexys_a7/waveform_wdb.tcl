# Build a Wave Config file from an existing static XSim waveform database.
if {[llength $argv] != 2} {
    error "usage: waveform_wdb.tcl <input.wdb> <output.wcfg>"
}

set script_dir [file dirname [file normalize [info script]]]
open_wave_database [lindex $argv 0]
source [file join $script_dir waveform.tcl]
show_selected_waves
save_wave_config [lindex $argv 1]
