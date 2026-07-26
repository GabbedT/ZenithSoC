# ZenithSoC documentation

The documentation site is built with Sphinx. The source is organized around
the way people usually approach the project:

```text
docs/
├── architecture/    system overview, integration status, bus, and memory map
├── cpu/             ApogeoRV and the cache complexes
├── audio/           capture, synthesis, and the APU
├── peripherals/     memory-mapped devices
└── legal/           license information
```

`index.rst` is the Sphinx entry point. Start with `architecture/overview.rst`
when you want the system-level picture, then follow the CPU and peripheral
pages for implementation details.

The RTL and shared packages remain the source of truth for interfaces, address
ranges, register offsets, and integration status. In particular, check
`hw/ZenithSoC.sv`, `hw/_zenithSoC.f`, `hw/utils/pkg/soc_parameters.sv`, and the
ApogeoRV sources under `hw/cpu/ApogeoRV/` when hardware and prose diverge.

## Build the site

Install the documentation dependencies once from the repository root:

```bash
python3 -m pip install -r docs/requirements.txt
```

Build the HTML site:

```bash
make -C docs html
# Open docs/_build/html/index.html
```

For a stricter check that treats warnings as errors:

```bash
make -C docs clean
make -C docs SPHINXOPTS=-W html
```

The optional live-reload workflow is:

```bash
sphinx-autobuild docs docs/_build/html
```

On Windows, run `docs/make.bat html` instead of the Make target.

## Update a page

1. Edit or add the `.rst` page beside the subsystem it describes.
2. Add new pages to the relevant `toctree` in `docs/index.rst`.
3. Derive register and address information from the RTL packages rather than
   copying an old table.
4. Run the strict build before handing the change off.

Generated files under `docs/_build/` are local build output and should not be
committed. Project compilation, firmware, simulation, co-simulation, and FPGA
commands are collected in the repository-level `README.md`.
