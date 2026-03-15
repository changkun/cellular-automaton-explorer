# Round 14: Frequency Analysis Overlay

## Goal
Add an oscillation frequency analysis overlay that reveals the hidden temporal structure of the automaton by color-coding every cell based on its detected oscillation period.

## What was built
- **Frequency analysis engine**: Per-cell period detection using autocorrelation over the last 64 frames of timeline history
- **Distinct color palette**: Ice blue (still life) -> emerald (p2) -> gold (p3) -> orange (p4-5) -> coral (p6-12) -> magenta (p13+) -> hot red (chaotic)
- **Smart rendering**: Currently-alive cells rendered brighter, "off" phase cells dimmer, showing full oscillation extent
- **Legend overlay**: Box in bottom-left showing color-to-period mapping
- **Auto-refresh**: Analysis recomputes every 8 generations while running
- **`f` key toggle**: Activates/deactivates the overlay

## Architecture decisions
- Leveraged existing 256-frame timeline ring buffer (exploitation) for historical analysis
- Autocorrelation with 85% match threshold for robust period detection
- Separate freq_grid array to cache results; freq_stale flag for lazy recomputation
- New cell type 7 (freq ghost) integrated into all three zoom-level renderers
- Analysis depth capped at 64 frames — balances accuracy vs. performance

## Exploration vs Exploitation balance (57%)
- **Exploration**: Entirely new analytical visualization paradigm — first mode that reveals temporal rather than spatial structure
- **Exploitation**: Built on timeline buffer (R10), rendering pipeline, cell_color system, overlay pattern (minimap/rule editor)

## Key files changed
- `life.c`: ~120 lines added (freq_grid, freq_analyze, freq_to_rgb, legend renderer, key handler, cell_color integration)
- `README.md`: New "Frequency Analysis Overlay" section with color table and usage tips
