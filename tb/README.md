# Testbenches and simulation models

`tb/` contains simulation-only infrastructure. It is not part of the synthesizable RTL source tree.

- `verilator/`: fast full-SoC Verilator testbench and firmware ELF loader.
- `top/`: traditional top-level testbench and trace artifacts.
- `ddr_model/`: DDR2 behavioral and golden models.
- `sd_model/`: SD-card and Wishbone-side behavioral models.

For the supported quick-start flow, read [`verilator/README.md`](verilator/README.md). Generated binaries, logs, waveforms, and Verilator build trees are ignored by Git.
