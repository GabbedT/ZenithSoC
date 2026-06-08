# ZenithSoC

> A complete System-on-Chip featuring a RISC-V CPU, comprehensive peripheral suite, and integrated audio processing

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![RISC-V](https://img.shields.io/badge/RISC--V-32bit-blue.svg)](https://riscv.org/)
[![SystemVerilog](https://img.shields.io/badge/Language-SystemVerilog-purple.svg)]()

## Overview

**ZenithSoC** is a high-performance System-on-Chip designed around the [ApogeoRV](Hardware/CPU/ApogeoRV) RISC-V processor core. It provides a complete platform for embedded systems development, featuring an extensive collection of peripherals, memory subsystems, and specialized processing units for audio applications.

The design emphasizes modularity, allowing developers to configure and synthesize only the components needed for their specific application. Optimized primarily for FPGA deployment (verified on Xilinx devices), ZenithSoC delivers an excellent balance of performance, resource utilization, and ease of integration.

## Key Features

### Core Architecture
- **CPU**: [ApogeoRV](Hardware/CPU/ApogeoRV) 32-bit RISC-V processor @ 100 MHz
  - ISA: RV32IMC + Zicsr, Zfinx, Zba, Zbs, partial Zbb
  - Out-of-order execution with in-order issue and writeback
  - Branch prediction (GSHARE + BTB)
  - Store buffer with load forwarding
  - Machine and User privilege modes
- **Memory System**
  - Separate instruction and data caches
  - On-chip memory for fast execution
  - DDR2 controller for external memory
  - Boot ROM support
- **Interconnect**: AXI4-Lite network

### Audio Processing Unit (APU)
- **Audio Capture**: PDM to PCM conversion with CIC filtering
- **Audio Synthesis**: 4-channel waveform generation
  - Sine, square, triangle, and custom waveform generators
  - ADSR envelope modulation
  - Multi-channel audio mixer
- **Integrated full-duplex audio pipeline**

### Communication Interfaces
- **UART**: Serial communication with hardware flow control (CTS/RTS)
- **SPI**: Master/slave serial peripheral interface
- **Ethernet**: 10/100 MAC controller with DMA support
- **SD Card**: SDIO/SPI interface with DMA capabilities

### Peripherals
- **GPIO**: 32 configurable I/O pins with interrupt support
- **Timer**: 64-bit timer with compare functionality
- **PWM**: Pulse-width modulation outputs
- **PRNG**: Hardware pseudo-random number generator
- **PDM2PCM**: Standalone PDM to PCM converter
- **VGA**: Video output controller (under integration)
- **Trace Unit**: Real-time instruction tracing for debugging

### Development Support
- **Interrupt Controller**: Centralized interrupt management
- **Debug Infrastructure**: Trace unit for execution monitoring
- Comprehensive documentation with register maps and programming examples

## Project Structure

```
ZenithSoC/
├── Hardware/
│   ├── CPU/
│   │   └── ApogeoRV/          # RISC-V processor core (submodule)
│   ├── Memory/
│   │   └── Cache/             # Instruction and data caches
│   ├── Bus/                   # AXI interconnect
│   ├── APU/                   # Audio Processing Unit
│   ├── IO/                    # Peripheral devices
│   │   ├── UART/
│   │   ├── SPI/
│   │   ├── Ethernet/
│   │   ├── GPIO/
│   │   ├── Timer/
│   │   ├── SD/
│   │   ├── PRNG/
│   │   ├── PWM/
│   │   ├── VGA/
│   │   └── PDM2PCM/
│   └── GPU/                   # Graphics Processing Unit (in development)
├── Software/
│   ├── Examples/              # Sample applications and drivers
│   └── Doc/                   # Software documentation and build instructions
├── Constraints/               # FPGA constraint files
└── Doc/                       # Hardware documentation (Sphinx)
```

## Documentation

Comprehensive documentation is available covering all subsystems:

- **[Online Documentation](Doc/_build/html/index.html)**: Complete hardware reference
  - System architecture
  - Register maps for all peripherals
  - Programming examples
  - Integration guidelines
- **[ApogeoRV CPU](Hardware/CPU/ApogeoRV/README.md)**: Detailed processor documentation
- **[Roadmap](ROADMAP.md)**: Feature status and development plan

### Building Documentation

```bash
cd Doc
make html
# Open Doc/_build/html/index.html in a browser
```

## Current Status

See [ROADMAP.md](ROADMAP.md) for complete feature list.

**Implemented and Verified:**
- Core processor with caches
- Memory subsystem with DDR2 support
- AXI interconnect
- UART, SPI, GPIO, Timer, Ethernet
- Audio Processing Unit (capture + synthesis)
- PRNG, PWM
- SD Card controller
- Trace unit

**In Development:**
- I²C interface
- I²S interface
- DMA controller
- VGA integration
- ADC system

## Examples

Explore practical examples in the `Software/Examples/` directory:

- **[Audio Record](Software/Examples/Audio%20Record/)**: Capture audio using the APU
- **[Ethernet](Software/Examples/Ethernet/)**: Network communication demos
- **[SPI](Software/Examples/SPI/)**: SPI device interfacing
- **[Debug Print](Software/Examples/Debug%20Print/)**: UART debugging utilities
- **[Floating Point](Software/Examples/Floating%20Point/)**: FPU usage with Zfinx
- **[PRNG](Software/Examples/PNRG/)**: Hardware random number generation

Each example includes a README with build and usage instructions.

## Contributing

Contributions are welcome! Whether you're fixing bugs, adding features, or improving documentation:

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Submit a pull request

Please ensure that:
- Code follows the existing style conventions
- New peripherals include documentation and register maps
- Hardware changes are verified through simulation
- Software examples are tested on hardware when possible

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Architecture Philosophy

ZenithSoC is designed with several core principles:

- **Modularity**: Each component can be included or excluded based on application needs
- **Performance**: Out-of-order CPU execution with intelligent caching
- **Ease of Use**: Well-documented register interfaces and comprehensive examples
- **Flexibility**: Highly configurable parameters for power, area, and performance tuning
- **Real-World Ready**: Includes practical peripherals for common embedded applications

## Support

For questions, bug reports, or feature requests:

- **Email**: tripi.gabriele2002@gmail.com
- **Issues**: Open an issue on GitHub
- **Documentation**: Start with the [documentation index](Doc/_build/html/index.html)