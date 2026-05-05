# ZenithSoC Documentation

This directory contains the Sphinx-based documentation for ZenithSoC.

## Building the Documentation

### Prerequisites

Install the required Python packages:

```bash
pip install -r requirements.txt
```

### Building HTML Documentation

On Linux/Mac:
```bash
make html
```

On Windows:
```bash
make.bat html
```

The generated HTML documentation will be in `_build/html/`. Open `_build/html/index.html` in a web browser to view it.

### Other Output Formats

Sphinx supports multiple output formats:

- `make html` - HTML pages
- `make latexpdf` - PDF via LaTeX
- `make epub` - EPUB e-book
- `make help` - List all available targets

## Documentation Structure

- `index.rst` - Main documentation index
- `introduction.rst` - Introduction and overview
- `architecture.rst` - SoC architecture description
- `memory_map.rst` - Memory address map
- `peripherals.rst` - Peripherals overview
- `axi.rst`, `uart.rst`, `spi.rst`, etc. - Individual peripheral documentation
- `conf.py` - Sphinx configuration
- `requirements.txt` - Python dependencies

## Live Preview

For automatic rebuilding during development:

```bash
sphinx-autobuild . _build/html
```

Then open http://127.0.0.1:8000 in your browser.

## Read the Docs Integration

This documentation is configured for Read the Docs hosting via `.readthedocs.yaml`.
