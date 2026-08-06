set script_dir [file dirname [file normalize [info script]]]

set project_file [file normalize \
    [file join $script_dir .. .. build vivado project ZenithSoC.xpr]]

set output_root  [file normalize [file join $script_dir ip]]
set ip_files_dir [file normalize [file join $output_root ip_user_files]]
set ipstatic_dir [file normalize [file join $ip_files_dir ipstatic]]
set xcelium_dir  [file normalize [file join $output_root xcelium_project]]

if {![file exists $project_file]} {
    error "Vivado project not found: $project_file"
}

puts "============================================================"
puts "Project:        $project_file"
puts "Output IP:      $ip_files_dir"
puts "Output Xcelium: $xcelium_dir"
puts "============================================================"

open_project $project_file

set ips [get_ips -quiet]

if {[llength $ips] == 0} {
    close_project
    error "No IP found!"
}

puts "\n=== IPs found ==="

foreach ip $ips {
    puts "  $ip"
    puts "    IPDEF: [get_property IPDEF $ip]"
}

# Clean previously generated outputs
file delete -force $ip_files_dir
file delete -force $xcelium_dir

file mkdir $output_root
file mkdir $ip_files_dir
file mkdir $ipstatic_dir
file mkdir $xcelium_dir

puts "\n=== Generating IP output products ==="

foreach ip $ips {
    puts "Generating: $ip"

    reset_target all $ip
    generate_target all $ip
}

puts "\n=== Exporting IP simulation packages ==="

foreach ip $ips {
    set ip_name [get_property NAME $ip]
    set ip_export_dir [file normalize \
        [file join $output_root $ip_name]]

    puts "Exporting Xcelium package for: $ip_name"
    puts "Destination: $ip_export_dir"

    file delete -force $ip_export_dir
    file mkdir $ip_export_dir

    # Export this IP's generated support files
    export_ip_user_files \
        -of_objects $ip \
        -ip_user_files_dir $ip_files_dir \
        -ipstatic_source_dir $ipstatic_dir \
        -no_script \
        -sync \
        -force

    # Generate the standalone Xcelium package for this IP
    export_simulation \
        -of_objects $ip \
        -simulator xcelium \
        -directory $ip_export_dir \
        -ip_user_files_dir $ip_files_dir \
        -ipstatic_source_dir $ipstatic_dir \
        -export_source_files \
        -report_ip_status \
        -verbose \
        -force
}

puts "\n============================================================"
puts "GENERATION COMPLETED"
puts "Shared IP files: $ip_files_dir"
puts "Output root:     $output_root"
puts "============================================================"

close_project
exit