# Round 53 — Phase Portrait Mode

**Explore/Exploit: 100% exploit**

## Goal
Plot two overlay scalar metrics against each other as a real-time 2D trajectory
in the sidebar, revealing attractor structure — limit cycles, fixed points,
bifurcations, chaotic wandering — that no single metric can show alone.

## What was built
- **Toggle**: `)` key (split-screen moved to `\`)
- **Axis cycling**: `<`/`,` cycles X-axis metric, `>`/`.` cycles Y-axis metric
- **16 available metrics**: Pop, Entropy, Lyapunov, Fractal D, Flow, Vortices,
  Wave Energy, dS/dt, β₀, β₁, ξ, Ising H, Magnetization, Complexity,
  Kolmogorov, Ergodicity
- **Sidebar panel** (~18 rows):
  - 30×12 Braille dot scatter plot (60×48 dot resolution)
  - 128-point trailing history with brightness fading (gold/amber palette,
    older = dimmer, newer = brighter)
  - Auto-scaling axes with 5% padding
  - Y-axis labels on left edge, X-axis range at bottom
  - Current (X, Y) values displayed
  - Axis metric names, cycling hints, and point count
- **Status bar**: `◎PHASE:Pop×Entropy` indicator in gold
- Stacks correctly below all other active overlay panels

## Key design decisions
- Pure exploitation: zero new analysis math, just combines existing scalar
  outputs from all overlays into a dynamical systems visualization
- `pp_read_metric()` switch statement reads from the same globals that
  individual overlays already compute — no duplicate calculations
- Ring buffer of 128 points with separate X/Y arrays for cache-friendly access
- Auto-range tracks global min/max with 5% padding; resets on metric change
- Braille encoding uses standard Unicode 2800-28FF block with correct
  dot-to-bit mapping (col0: rows 0-2→bits 0-2, row3→bit6; col1: rows 0-3→bits 3-5,7)
- Records every 2 generations to provide useful temporal spacing
- Brightness gradient: linear ramp from 40 to 255 across the history buffer

## Keybinding change
- `)` reassigned from split-screen to phase portrait (more intuitive — "round"
  symbol for phase space)
- Split-screen moved to `\` (backslash)

## Files changed
- `life.c` — all changes in single file: data structures, metric reader,
  recording logic, Braille renderer, key handlers, status bar indicator
