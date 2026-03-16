# Round 50: Spacetime Kymograph Mode

## Goal
Add a kymograph visualization that displays a 1D horizontal slice of the grid evolving over time as a 2D spacetime diagram. The first overlay to treat time as a spatial dimension — a fundamentally different paradigm from all existing overlays.

## What was built

### Spacetime Kymograph (Toggle: `K`)
- **Ring buffer**: 256-generation deep history of a single grid row, recorded every generation
- **Half-block rendering**: Uses ▀ characters to pack 2 time-steps per terminal row, achieving 512 generations visible on a 256-row terminal
- **Thermal gradient coloring**: Cell age mapped to blue→cyan→green→yellow→red→white gradient; dead cells rendered as near-black for high contrast
- **Time axis**: Left gutter shows generation-offset labels every 10 rows; time flows upward (newest at bottom)
- **Scan row selection**: Arrow Up/Down moves which grid row is sampled; buffer clears on row change for a clean view
- **Viewport panning**: Left/Right arrows pan the X-axis across the full 400-cell grid width

### What the kymograph reveals
- **Gliders/spaceships**: Appear as diagonal lines — slope encodes velocity (c/1 = 45°, c/2 = steeper, etc.)
- **Oscillators**: Vertical striped columns — visible period as the repeat distance in the Y (time) direction
- **Still lifes**: Solid vertical bars (unchanging across time)
- **Chaotic regions**: Textured noise — no discernible periodicity
- **Wavefronts**: Expanding V-shapes from births propagating outward

### Info Panel (top-right)
- Row 1: Current scan row number and grid height
- Row 2: Buffer depth (filled entries out of 256 max)
- Row 3: Row population sparkline — 24-character history of how many cells are alive in the scan row over time
- Row 4: Axis legend (↕ row, X=space, Y=time↑)
- Row 5: Key hints

### Integration
- Status bar indicator: `◇KYMO:rowN` (cyan) showing active scan row
- Help bar: `[K]kymo` added to key listing
- Demo reset: kymo_mode properly cleared on demo scene transitions
- Clean compilation with -O2 -Wall (no new warnings)

## Technical notes
- The kymograph is a **mode** (like dashboard or minimap), not a cell-color overlay, since it replaces the grid view entirely rather than recoloring cells in place
- Buffer uses `unsigned char kymo_buf[256][400]` — 100KB static allocation, consistent with the project's no-malloc philosophy
- Half-block rendering achieves 2x vertical density, making glider worldlines clearly visible even in small terminals
- Row population tracking stored alongside the ring buffer for the sparkline display

## Lines added
~170 lines (state variables, recording, rendering, info panel, key handler, status bar)
