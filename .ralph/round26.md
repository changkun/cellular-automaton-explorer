# Round 26: Shannon Entropy Heatmap — Spatial Information Analysis

## Goal
Add a real-time Shannon entropy visualization that computes local information entropy over each cell's Moore neighborhood, revealing order/chaos boundaries, information flow corridors, and complexity gradients.

## What Was Built

### Entropy Computation
- Per-cell Shannon entropy H = -p·log₂(p) - (1-p)·log₂(1-p) over 3×3 Moore neighborhood
- Topology-aware neighbor lookup (works with torus, Klein bottle, Möbius, projective plane)
- Result normalized to [0.0, 1.0]: 0 = uniform (all alive or all dead), 1.0 = maximum entropy (half and half)
- Auto-recomputes every 4 generations when active (lightweight compared to frequency analysis)
- Tracks global mean entropy and maximum local entropy

### Visualization
- Distinct green → white → magenta palette:
  - Dark green: low entropy (ordered regions, interior of structures)
  - White: medium entropy (boundaries, edges of structures)
  - Magenta: high entropy (chaotic regions, mixing zones)
- Alive cells brightened for structure visibility through overlay
- Dead cells with non-zero entropy shown as dim ghosts (reveals neighborhood influence)

### Overlay Panel
- Positioned top-right corner, compact 44-wide box
- Shows H̄ (mean entropy) and H_max (maximum local entropy)
- 10-bin distribution histogram with 4-row vertical bar chart
- Bars colored using the same entropy palette for visual consistency
- Unicode box-drawing borders in green theme

### Controls
- `i` / `I`: Toggle entropy heatmap mode
- HUD indicator shows `∴ENT:0.XXXX` with current global mean

## Technical Details
- `entropy_grid[MAX_H][MAX_W]` stores float entropy per cell
- `entropy_compute()` iterates full grid, O(W×H×9) per refresh
- `entropy_to_rgb()` maps [0,1] → RGB via two-segment linear interpolation
- Integrated into `cell_color()` rendering pipeline (priority between freq and census)
- Staleness tracking: `entropy_stale` flag set on every grid_step, cleared on compute

## Files Modified
- `life.c`: +205 lines (globals, compute function, color mapping, cell_color hook, overlay panel, key binding, HUD indicator, help bar)
