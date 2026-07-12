# Hardware utilities

This directory contains shared SystemVerilog interfaces, packages, and tool definitions. Packages under `pkg/` define SoC parameters, bus types, peripheral registers, caches, tracing, and audio; `intf/` contains reusable AXI interfaces; `defs/` contains synthesis-tool definitions.

These files are shared broadly, so interface changes should be reviewed together with all affected RTL, software drivers, documentation, and simulation flows.
