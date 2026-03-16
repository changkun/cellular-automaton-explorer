# Round 76 — Poincaré Recurrence Time Map Overlay

## Goal
Add a Poincaré recurrence time map that tracks per-cell 3×3 neighborhood microstates and measures how many frames elapse before exact recurrence, creating a "predictability landscape" heatmap.

## What was built

**Overlay #42** — Poincaré Recurrence Time Map (`Ctrl-Y`, ghost code 43)

### Components

1. **State declarations** (~55 lines) — Per-cell direct-mapped hash table (8 slots × 80K cells) storing (microstate, frame) pairs, EMA-smoothed recurrence times, recurrence flags, frame counter, global statistics (mean τ, max τ, fraction recurred), sparkline histories, classification state.

2. **Compute function** `pr_compute()` (~100 lines) — Per-frame algorithm:
   - Phase 1: For each cell, compute 9-bit microstate from 3×3 Moore neighborhood (including center cell)
   - Phase 2: Look up microstate in direct-mapped hash slot (slot = ms & 7); if match found, compute recurrence time as frame delta
   - Phase 3: Store current microstate and frame in slot (overwrites on collision — LRU-like)
   - Phase 4: EMA-smooth recurrence time per cell (α=0.85)
   - Phase 5: Compute global stats: mean τ, max τ, fraction of grid that has recurred
   - Phase 6: Classify dynamics regime (CRYSTALLINE < 3, PERIODIC < 10, MIXING < 50, CHAOTIC ≥ 50)
   - Phase 7: Normalize per-cell values to [0,1] using log scale for rendering
   - Phase 8: Update sparkline ring buffers

3. **Grid rendering** (~50 lines) — Six-band color palette:
   - Deep blue → cyan (v < 0.15): very fast recurrence (period-1/2 patterns)
   - Cyan → teal-green (0.15–0.35): fast recurrence
   - Green → yellow (0.35–0.55): moderate recurrence
   - Yellow → orange-red (0.55–0.75): long recurrence
   - Red → hot pink (0.75–0.90): very long recurrence
   - White-hot (> 0.90): never-yet-recurred / extreme
   - Alive cells brightened by +55 per channel

4. **Sidebar panel** (9 rows) — Shows:
   - Mean recurrence time τ̄ with dynamics classification
   - Maximum observed τ and percentage of grid that has recurred
   - Frame counter and percentage of grid still novel (never recurred)
   - Color scale legend with [^Y] keybinding hint
   - Two sparklines: mean τ history and fraction-recurred history

### Integration points
- Ghost code registration: `pr_mode → 42` (line ~2226)
- Stale flag: `pr_stale = 1` after each generation (line ~3591)
- Compute dispatch: `case 42` in overlay switch (line ~11225)
- Cell rendering: after `ir_mode` block in `cell_color()` (line ~13338)
- Sidebar: after Interface Roughness panel, before Percolation panel
- Stacking: perc_mode panel updated to include `pr_mode += 9`
- Key binding: `key == 25` (Ctrl-Y) toggles `pr_mode`

## Mathematical basis
Poincaré's recurrence theorem guarantees that every bounded dynamical system eventually returns arbitrarily close to any prior state. In a finite cellular automaton with 512 possible 3×3 microstates per cell, exact recurrence is guaranteed but the timescale varies enormously: period-2 blinkers recur in 2 frames while chaotic regions may take hundreds. The overlay makes this timescale variation directly visible as a spatial heatmap.

## Memory budget
- Hash table: 200×400×8×(2+4) bytes = ~3.66 MB
- Per-cell floats: 200×400×(4+4+1) bytes = ~0.72 MB
- Total: ~4.4 MB static allocation
