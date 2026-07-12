# Audio Processing Unit

The APU combines audio capture and synthesis blocks behind a memory-mapped interface. The top-level integration is [`apu.sv`](apu.sv), with source membership maintained in [`_apu.f`](_apu.f).

- `capture_unit/`: CIC filtering and recorder pipeline.
- `synthesis_unit/`: waveform generators, ADSR modulation, mixing, and synthesis pipeline.
- `intf/`: registers and PDM/PCM conversion interfaces.

The register-level reference is in [`docs/apu/`](../../docs/apu/). Keep the package definitions and documentation synchronized when adding channels, registers, or sample-format changes.
