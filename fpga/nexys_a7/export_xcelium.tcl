set project_file [file normalize "../../build/vivado/ZenithSoC/ZenithSoC.xpr"]
set ip_dir       [file normalize "./ip"]
set export_dir   [file normalize "./ip/xcelium"]

if {![file exists $project_file]} {
    error "Progetto Vivado non trovato: $project_file"
}

open_project $project_file

set ips [get_ips -quiet]

puts "=== IP trovate ==="
foreach ip $ips {
    puts "  $ip - [get_property VLNV $ip]"
}

if {[llength $ips] == 0} {
    error "Nessuna IP trovata"
}

file delete -force $ip_dir
file mkdir $ip_dir
file mkdir $export_dir

foreach ip $ips {
    puts "Generazione: $ip"
    generate_target all $ip
}

export_ip_user_files \
    -of_objects $ips \
    -ip_user_files_dir "$ip_dir/ip_user_files" \
    -ipstatic_source_dir "$ip_dir/ip_user_files/ipstatic" \
    -no_script \
    -sync \
    -force

update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

export_simulation \
    -of_objects [get_filesets sim_1] \
    -simulator xcelium \
    -directory $export_dir \
    -ip_user_files_dir "$ip_dir/ip_user_files" \
    -ipstatic_source_dir "$ip_dir/ip_user_files/ipstatic" \
    -export_source_files \
    -force

puts "Export completato in $ip_dir"

close_project
exit
