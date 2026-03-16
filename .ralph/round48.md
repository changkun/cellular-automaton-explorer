# Round 48: Split-Screen Dual Overlay Comparison

## Goal
Add a split-screen mode that divides the viewport vertically, rendering two different analysis overlays side-by-side on the same live simulation. Pure exploitation — multiplies the value of all 33 existing overlays by enabling real-time cross-domain correlation.

## What was built

### Split-Screen Mode (Toggle: `)`)
- Divides the zoom==1 viewport into left and right halves separated by a thin vertical divider
- Each half renders the same simulation grid through a different overlay lens
- **Left panel**: initialized to whatever overlay was active when entering split mode
- **Right panel**: defaults to the next overlay in the cycle
- **TAB**: cycles the right panel through all 21 overlay options (None + 20 analysis overlays)
- **Backtick (`)**: cycles the left panel through all overlays
- Press `)` again to exit split mode and restore normal rendering

### Overlay Table
Defined a unified overlay index system mapping all 21 overlay modes:
- 0: None (plain cells), 1: Frequency, 2: Entropy, 3: Lyapunov, 4: Fourier
- 5: Fractal, 6: Flow, 7: Attractor, 8: Surprise, 9: Mutual Info
- 10: Complexity, 11: Topology, 12: RG Flow, 13: Kolmogorov, 14: Correlation
- 15: dS/dt, 16: Vorticity, 17: Wave, 18: Ergodicity, 19: Wolfram, 20: Census

### Implementation approach
- `split_set_overlay()`: saves/restores all 20 overlay mode flags, enabling exactly one for rendering each half
- `split_detect_current()`: detects which overlay is active to initialize panels intelligently
- `split_ensure_computed()`: ensures data is computed for both panels before rendering
- `EMIT_CELL_Z1` macro: factored out the cell-rendering logic to avoid duplicating the complex type-dispatch code
- Overlay stats panels are suppressed in split mode (they'd overlap and be incoherent)
- Status bar shows `SPLIT:Left|Right` indicator with overlay names

### Design decisions
- Split-screen only works at zoom==1 (the default/most common). Higher zoom levels have more complex per-char rendering that would need separate macros.
- Overlay detail panels are hidden in split mode — the point is comparative visualization, not detailed stats
- Labels appear at the top of each half panel identifying the overlay

## Lines added
+256 (12,756 -> 13,012)

## Exploration/Exploitation
0% explore / 100% exploit — no new analysis, pure leverage of existing overlays
