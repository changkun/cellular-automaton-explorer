---
round: 6
goal: "Zoom and pan — multi-scale viewport with half-block and quarter-block rendering"
status: complete
exploration_pct: 0
---

# Round 6: Zoom & Pan Navigation

Decoupled the simulation grid from the terminal viewport, enabling users to explore the full 400×200 grid at three zoom levels with smooth panning.

## What was built

**Zoom levels** (`z` zoom in, `x` zoom out, mouse scroll):
- **1x** (default) — 2 terminal chars per cell, 1 row per cell (same as before)
- **2x** — Unicode half-blocks (▀▄) pack 2 vertical cells per character, 1 char wide per cell — 4x density
- **4x** — Unicode quarter-blocks (▘▝▖▗▀▄▌▐ + 8 more) pack 4 cells per character — 16x density with averaged heatmap colors

**Pan** (arrow keys): scroll viewport across the grid in steps that scale with zoom level (4/8/16 cells). Press `0` to re-center.

**Architecture changes:**
- Grid is now always MAX_W×MAX_H (400×200) regardless of terminal size
- New viewport system: `view_x`, `view_y`, `view_w`, `view_h` track the visible region
- `viewport_update()` recalculates cell-fitting on resize/zoom
- Terminal resize no longer truncates the grid — it just adjusts the viewport
- `cell_color()` helper unifies heatmap/flat-color logic for all zoom renderers
- Mouse coordinates translated through viewport offset + zoom scale
- Arrow key parsing added to CSI input handler (was previously ignored)

**Rendering detail:**
- Zoom 2x: Uses ▀ with fg=top cell, bg=bottom cell for full-color accuracy
- Zoom 4x: 16 quadrant block characters for all 4-bit alive/dead combinations, with averaged RGB from alive cells in heatmap mode
- Ghost trails visible at all zoom levels

## Why this matters

The simulation always had room for 400×200 cells but was limited to what fit on screen. Now:
- Gosper Gun streams are visible for hundreds of cells at 4x zoom
- R-pentomino and Acorn methuselahs can be watched spreading across the full grid
- Kaleidoscope art at 8-fold symmetry reveals intricate macro structure at 4x zoom
- Drawing at 1x then zooming out shows how micro patterns compose into macro behavior
