# ZenithSoC documentation

The documentation is built with Sphinx. It covers the SoC architecture, CPU complex, AXI interconnect, memory map, peripherals, and audio processing unit. The source index is [`index.rst`](index.rst); generated HTML belongs in `_build/html/` and should not be edited by hand.

## Install and build

From the repository root:

```bash
python3 -m pip install -r docs/requirements.txt
cd docs
make html
```

Open `docs/_build/html/index.html`. For a strict build suitable for CI:

```bash
python3 -m sphinx -W -b html . _build/html
```

For live editing, install `sphinx-autobuild` from `requirements.txt` and run:

```bash
sphinx-autobuild . _build/html
```

Other Sphinx targets are available through `make help`, including `latexpdf` and `epub`. On Windows use `make.bat html`.

## Source layout

- `cpu_complex/`, `io_devs/`, and `apu/`: subsystem reference pages.
- `conf.py`: Sphinx configuration and extensions.
- `requirements.txt`: pinned documentation dependencies.
- `DOCUMENTATION_SUMMARY.md`: overview of the current documentation set.

When changing a register map or memory address, update both the RTL packages and the relevant RST page.
