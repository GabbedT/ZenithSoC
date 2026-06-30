../../hw/utils/defs/vivado.svh

../../hw/cpu/ApogeoRV/hw/inc/headers/apogeo_memory_map.svh

../../hw/utils/pkg/apu_pkg.sv
../../hw/utils/pkg/axi_interface_pkg.sv
../../hw/utils/pkg/cache_pkg.sv
../../hw/utils/pkg/ethernet_pkg.sv
../../hw/utils/pkg/sd_pkg.sv
../../hw/utils/pkg/soc_parameters.sv
../../hw/utils/pkg/spi_pkg.sv
../../hw/utils/pkg/trace_unit_pkg.sv
../../hw/utils/pkg/uart_pkg.sv
../../hw/utils/pkg/vga_pkg.sv

../../hw/utils/intf/axi_read_interface.sv
../../hw/utils/intf/axi_write_interface.sv

-F ../../hw/cpu/_cpu.f
-F ../../hw/common/_common.f
-F ../../hw/apu/_apu.f

../../hw/memory/ddr/cache_ddr_interface.sv

../../hw/memory/memory_bank.sv
../../hw/memory/on_chip_memory.sv

../../hw/cpu/cpu_complex.sv
../../hw/cpu/interrupt_controller.sv

-F ../../hw/bus/_bus.f

-F ../../hw/io/ethernet/_ethernet.f
-F ../../hw/io/sd/_sd.f
-F ../../hw/io/spi/_spi.f
-F ../../hw/io/uart/_uart.f

../../hw/io/gpio/gpio.sv
../../hw/io/prng/prng.sv
../../hw/io/timer/timer.sv

../../hw/cpu/trace_unit/trace_unit_packetizer.sv
../../hw/cpu/trace_unit/trace_unit_serializer.sv
../../hw/cpu/trace_unit/trace_unit_registers.sv
../../hw/cpu/trace_unit/trace_unit.sv

../../hw/ZenithSoC.sv

zenith_tb_top.sv
zenith_public.vlt