# ZenithSoC Documentation Build Instructions

## Prerequisites

- Python 3.12 or higher
- pip (Python package manager)

## Installation

Install the required Python packages:

```bash
cd Doc
pip install -r requirements.txt
```

## Building the Documentation

### Using Sphinx directly

```bash
cd Doc
python -m sphinx -b html . _build/html
```

### Using Make (Linux/macOS)

```bash
cd Doc
make html
```

### Using make.bat (Windows)

```cmd
cd Doc
make.bat html
```

## Viewing the Documentation

After building, open the generated HTML in your browser:

- **Main page**: `Doc/_build/html/index.html`

Or start a local web server:

```bash
cd Doc/_build/html
python -m http.server 8000
```

Then navigate to `http://localhost:8000` in your browser.

## Directory Structure

```
Doc/
├── index.rst                    # Main documentation index
├── conf.py                      # Sphinx configuration
├── requirements.txt             # Python dependencies
├── Makefile                     # Unix build script
├── make.bat                     # Windows build script
├── .readthedocs.yaml           # Read the Docs configuration
│
├── APU/                         # Audio Processing Unit
│   ├── apu.rst                 # APU overview
│   ├── audio_capture_unit.rst  # Audio capture documentation
│   └── audio_synthesis_unit.rst# Audio synthesis documentation
│
├── CPU Complex/                 # CPU and caches
│   ├── cpu_complex.rst         # CPU complex overview
│   ├── instruction_cache.rst   # I-cache documentation
│   └── data_cache.rst          # D-cache documentation
│
├── IO Devices/                  # Peripherals
│   ├── gpio.rst                # GPIO controller
│   ├── uart.rst                # UART controller
│   ├── spi.rst                 # SPI controller
│   ├── timer.rst               # Timer peripheral
│   ├── ethernet.rst            # Ethernet MAC
│   ├── sd.rst                  # SD card controller
│   ├── prng.rst                # Random number generator
│   └── trace_unit.rst          # Debug trace unit
│
├── axi_bus.rst                  # AXI interconnect
├── license.rst                  # License information
│
├── _build/                      # Generated documentation (gitignored)
├── _static/                     # Static assets (CSS, images)
└── _templates/                  # Custom templates
```

## Documentation Coverage

### Completed Documentation

1. **System Architecture**
   - CPU Complex (ApogeoRV RV32IMFZfinx)
   - Instruction Cache (direct-mapped with FSM)
   - Data Cache (separate load/store controllers)
   - AXI Bus (interconnect and arbitration)

2. **Audio Processing Unit**
   - APU Overview (integrated capture + synthesis)
   - Audio Capture Unit (PDM to PCM conversion)
   - Audio Synthesis Unit (4-channel waveform generation)

3. **IO Devices**
   - GPIO (32-pin configurable)
   - UART (hardware flow control)
   - SPI (master/slave modes)
   - Timer (64-bit with compare)
   - Ethernet MAC (DMA support)
   - SD Card (SDIO/SPI interface)
   - PRNG (hardware random generator)
   - Trace Unit (instruction tracing)

4. **Legal**
   - MIT License information

## Build Output

The HTML documentation is generated in `Doc/_build/html/` with the following structure:

- `index.html` - Main landing page
- `APU/` - Audio processing documentation
- `CPU Complex/` - CPU and cache documentation
- `IO Devices/` - Peripheral documentation
- `_static/` - CSS, JavaScript, fonts
- `_sources/` - RST source files (for reference)

## Known Issues

### Warnings

The build generates warnings about duplicate labels (e.g., "Overview", "Architecture", "Features"). These are harmless and do not affect the generated documentation. Each document section is still accessible and correctly rendered.

To suppress warnings, use:

```bash
python -m sphinx -b html -q . _build/html
```

### Table Formatting (SD Card Controller)

The SD card documentation has 2 table formatting warnings. These are due to long descriptions that exceed column width. The tables still render correctly in HTML.

## Continuous Integration

The documentation is configured for Read the Docs deployment via `.readthedocs.yaml`.

### Read the Docs Configuration

- **OS**: Ubuntu 22.04
- **Python**: 3.12
- **Configuration**: conf.py
- **Requirements**: requirements.txt

To deploy on Read the Docs:

1. Connect your repository to Read the Docs
2. The build will automatically use `.readthedocs.yaml` configuration
3. Documentation will be available at `https://your-project.readthedocs.io`

## Customization

### Theme

The documentation uses the Sphinx Read the Docs theme (`sphinx_rtd_theme`). To change the theme, edit `conf.py`:

```python
html_theme = 'alabaster'  # or any other Sphinx theme
```

### Extensions

Current extensions in `conf.py`:

- `sphinx.ext.todo` - TODO directives
- `sphinx.ext.autosectionlabel` - Automatic section labels
- `sphinx.ext.autodoc` - Auto-documentation from docstrings

To add extensions, edit the `extensions` list in `conf.py`.

### Static Assets

Add custom CSS, images, or JavaScript to the `_static/` directory and reference them in `conf.py`:

```python
html_css_files = ['custom.css']
html_js_files = ['custom.js']
```

## Troubleshooting

### "No module named sphinx"

Install Sphinx:

```bash
pip install sphinx
```

### "Theme 'sphinx_rtd_theme' not found"

Install the theme:

```bash
pip install sphinx_rtd_theme
```

### Build fails with encoding errors

Ensure all RST files are UTF-8 encoded.

### Missing index.html

Check for syntax errors in RST files:

```bash
python -m sphinx -b html -W . _build/html
```

The `-W` flag treats warnings as errors.

## Development Workflow

1. Edit RST files in the `Doc/` directory
2. Rebuild documentation: `make html` or `python -m sphinx -b html . _build/html`
3. Refresh browser to see changes
4. For live reload during development:

```bash
sphinx-autobuild . _build/html
```

This starts a local server with automatic rebuild on file changes.

## Contributing

When adding new documentation:

1. Create RST file in appropriate subdirectory
2. Add entry to `index.rst` toctree
3. Follow existing documentation style (precise and concise)
4. Include programming examples where applicable
5. Test build before committing

## Support

For Sphinx documentation syntax, see:
- [Sphinx Documentation](https://www.sphinx-doc.org/)
- [reStructuredText Primer](https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html)

For ZenithSoC-specific questions, refer to the generated documentation or contact the maintainer.
