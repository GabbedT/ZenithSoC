# AXI bus

This directory implements the SoC’s AXI interconnect and endpoint interfaces. `axi_network.sv` composes the fabric, while `axi_master.sv`, `axi_slave.sv`, and `axi_router.sv` provide the principal routing and endpoint logic. The source list is [`_bus.f`](_bus.f).

See [`docs/io_devs/axi_bus.rst`](../../docs/io_devs/axi_bus.rst) for the documented protocol and integration assumptions.
