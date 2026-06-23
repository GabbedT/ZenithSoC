utils/defs/vivado.svh

cpu/ApogeoRV/hw/inc/headers/apogeo_memory_map.svh

utils/intf/axi_read_interface.sv
utils/intf/axi_write_interface.sv

utils/pkg/apu_pkg.sv
utils/pkg/axi_interface_pkg.sv
utils/pkg/cache_pkg.sv
utils/pkg/ethernet_pkg.sv
utils/pkg/sd_pkg.sv
utils/pkg/soc_parameters.sv
utils/pkg/spi_pkg.sv
utils/pkg/trace_unit_pkg.sv
utils/pkg/uart_pkg.sv
utils/pkg/vga_pkg.sv

-F common/_common.f


memory/ddr/ddr_memory_interface.sv
memory/ddr/cache_ddr_interface.sv

memory/memory_bank.sv
memory/on_chip_memory.sv

-F cpu/_cpu.f
-F apu/_apu.f

-F bus/_bus.f

-F io/ethernet/_ethernet.f
-F io/sd/_sd.f
-F io/spi/_spi.f
-F io/uart/_uart.f

io/gpio/gpio.sv
io/prng/prng.sv
io/timer/timer.sv

cpu/trace_unit/trace_unit_packetizer.sv
cpu/trace_unit/trace_unit_serializer.sv
cpu/trace_unit/trace_unit_registers.sv
cpu/trace_unit/trace_unit.sv