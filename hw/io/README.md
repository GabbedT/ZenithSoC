# I/O peripherals

Each subdirectory contains one memory-mapped peripheral and, where applicable, its local filelist and register implementation:

`ethernet`, `gpio`, `prng`, `sd`, `spi`, `timer`, `uart`, and `vga`.

The peripheral reference pages are in [`docs/io_devs/`](../../docs/io_devs/), and software drivers are in [`sw/lib/driver/`](../../sw/lib/driver/) with implementations in [`sw/src/`](../../sw/src/). When adding or changing a peripheral, update its RTL, register package/map, driver, documentation, and a testbench or VP block test.
