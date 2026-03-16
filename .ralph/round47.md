# Round 47: Ergodicity Metric Overlay

**Date**: 2026-03-16
**Explore/Exploit**: 68% explore / 32% exploit

## Goal
Measure whether the cellular automaton has reached statistical equilibrium by
comparing per-cell time-averaged density against the global spatial average.
Ergodic systems have time-avg ≈ space-avg everywhere; broken ergodicity reveals
frozen structures, absorbing states, and persistent spatial bias. This fills a
fundamental gap: all existing overlays measure instantaneous or short-window
properties, but none answer "has this system equilibrated?"

## What was built

### Ergodicity Metric (`(` key)
- **Per-cell time average**: Exponential moving average (EMA) of cell density
  with adaptive alpha: fast convergence during warmup (α=0.1 for first 50
  frames, α=0.05 for 50-200, α=0.02 steady state)
- **Deviation score**: |⟨density⟩_time − ⟨density⟩_space| per cell
- **Ergodicity index**: 1 − 4·mean_deviation, clamped to [0,1]. 1=fully
  ergodic, 0=completely broken
- **Color scheme**: Green (ergodic, low deviation) → yellow (mild) → magenta
  (strongly non-ergodic/frozen) with smooth 4-tier gradient
- **Live cells brightened**: +55 to all channels
- **Equilibration tracking**: Records generation when index first exceeds 0.9,
  resets if it drops below 0.85
- **State reset**: EMA and warmup counters clear on grid_clear/grid_randomize
  to prevent stale averages from persisting

### Stats Panel (9 rows, green theme)
- Ergodicity index with color-coded severity (green/yellow/magenta)
- Fraction of ergodic cells (deviation < 0.08 threshold)
- Spatial average density (ρ̄) and equilibration generation
- Color gradient legend: ergodic ↔ non-ergodic
- Ergodicity index sparkline (64-frame history buffer)
- Phase classification: Warming Up / Ergodic / Weakly Ergodic / Partially
  Broken / Broken Ergodicity

### Probe Inspector
- New row showing per-cell deviation and time-averaged density (ρ̄)

## Integration points
- Toggle key: `(`
- Stale flag: `ergo_stale` set in `update()`, auto-refreshed every 2 generations
- Demo overlay switch: supports `(` in overlay strings
- Demo reset: `ergo_mode = 0`
- Grid clear: resets EMA, warmup, equilibration, sparkline history

## Lines
12,433 → 12,756 (+323 lines)

## What's interesting
- Conway's Life with random initial conditions is generally non-ergodic: still
  lifes and oscillators create permanently biased regions
- Rules like Anneal or Day & Night tend toward higher ergodicity
- Temperature mode interaction: stochastic noise should push systems toward
  ergodicity, visible as the green overlay spreading
- Connects to the entropy production overlay: ordering (blue dS/dt) should
  correlate with non-ergodic regions (magenta ergodicity)
