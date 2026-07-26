# Write an UpdateMEM memory map for the four byte-wide RAMB36E1 blocks that
# implement the 16 KiB boot memory.  This must run against an implemented
# design because block-RAM locations are assigned during placement.

proc write_bootloader_mmi {output_file part} {
    set boot_rams [get_cells -hierarchical -filter {
        REF_NAME == RAMB36E1 && NAME =~ "*boot_memory/mem_bank*"
    }]

    if {[llength $boot_rams] != 4} {
        error "Expected four RAMB36E1 boot-memory cells, found [llength $boot_rams]: $boot_rams"
    }

    array set placement_by_bank {}
    foreach ram $boot_rams {
        set ram_name [get_property NAME $ram]
        if {![regexp {mem_bank\[([0-3])\]} $ram_name -> bank]} {
            error "Cannot determine the byte lane from boot-memory cell '$ram_name'"
        }
        if {[info exists placement_by_bank($bank)]} {
            error "More than one boot-memory RAM was found for byte lane $bank"
        }

        set placement [get_property LOC $ram]
        if {$placement eq ""} {
            error "Boot-memory cell '$ram_name' has not been placed"
        }
        regsub {^RAMB36_} $placement {} placement
        set placement_by_bank($bank) $placement
    }

    for {set bank 0} {$bank < 4} {incr bank} {
        if {![info exists placement_by_bank($bank)]} {
            error "Boot-memory byte lane $bank was not found"
        }
    }

    set output [open $output_file w]
    puts $output {<?xml version="1.0" encoding="UTF-8"?>}
    # Minor version 6 uses the legacy homogeneous-memory schema understood by
    # both Vivado 2024.1 and newer UpdateMEM releases.
    puts $output {<MemInfo Version="1" Minor="6">}
    puts $output {  <Processor Endianness="Little" InstPath="zenith_cpu">}
    puts $output {    <AddressSpace Name="boot_memory" Begin="0" End="16383">}
    puts $output {      <BusBlock>}
    for {set bank 0} {$bank < 4} {incr bank} {
        set lsb [expr {$bank * 8}]
        set msb [expr {$lsb + 7}]
        puts $output "        <BitLane MemType=\"RAMB36\" Placement=\"$placement_by_bank($bank)\">"
        puts $output "          <DataWidth MSB=\"$msb\" LSB=\"$lsb\"/>"
        puts $output {          <AddressRange Begin="0" End="4095"/>}
        puts $output {          <Parity ON="false" NumBits="0"/>}
        puts $output {        </BitLane>}
    }
    puts $output {      </BusBlock>}
    puts $output {    </AddressSpace>}
    puts $output {  </Processor>}
    puts $output {  <Config>}
    puts $output "    <Option Name=\"Part\" Val=\"$part\"/>"
    puts $output {  </Config>}
    puts $output {  <DRC>}
    puts $output {    <Rule Name="RDADDRCHANGE" Val="false"/>}
    puts $output {  </DRC>}
    puts $output {</MemInfo>}
    close $output

    puts "Bootloader memory map: $output_file"
}
