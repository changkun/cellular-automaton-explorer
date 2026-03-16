# Round 74 — Order Parameter Susceptibility Overlay

## Goal
Add a local susceptibility field χ(x,y) = N·Var(ρ) that measures the variance of the order parameter (density) in sliding spatial patches over a rolling time window, producing a spatial map of phase-transition sensitivity.

## What was built

**Overlay #40** — Order Parameter Susceptibility (`Ctrl-U`, ghost code 41)

### Components

1. **State declarations** (~60 lines) — Per-patch rolling statistics using ring buffer for exact rolling window variance over 64 frames. Patch grid of 8×8 cells. Per-cell interpolated susceptibility for smooth rendering.

2. **Compute function** `su_compute()` (~100 lines) — Computes local density ρ per patch each frame, stores in ring buffer, then calculates temporal variance and susceptibility χ = N·Var(ρ). Also computes Binder cumulant U₄ = 1 − ⟨m⁴⟩/(3⟨m²⟩²) as a universal crossing diagnostic. Interpolates patch-level χ to per-cell for smooth heatmap rendering.

3. **Grid rendering** (~50 lines) — Five-band color gradient: deep cold blue (rigid/ordered) → teal → yellow-green (approaching criticality) → orange-red (high susceptibility) → white-hot (critical/divergent). Normalized to current max χ for adaptive contrast.

4. **Sidebar panel** (9 rows) — Shows:
   - Global mean χ̄ with classification (ORDERED/MODERATE/SENSITIVE/CRITICAL)
   - Peak χ value and grid location
   - Binder cumulant U₄
   - Color scale legend
   - Two sparklines: χ(t) time series and U₄(t) time series

5. **Keyboard handler** — `Ctrl-U` toggle with flash message

6. **Panel stacking** — Integrated into all downstream panels (percolation)

### Algorithm

```
For each frame:
  1. For each 8×8 patch, count alive cells → density ρ
  2. Store ρ in ring buffer (64-frame window)
  3. Once window full:
     a. Compute mean and variance of ρ over window
     b. χ_patch = 64 · Var(ρ)  [fluctuation-dissipation form]
     c. Compute ⟨m²⟩ and ⟨m⁴⟩ for Binder cumulant
  4. Interpolate patch χ → per-cell χ
  5. Update sparkline history
```

### Integration points in life.c

- State declarations: after Spectral Gap block (line ~1803)
- Ghost code: `if (su_mode) return 40;` (line ~2020)
- Stale flag: `su_stale = 1;` in grid step (line ~3382)
- Compute: `case 40: if (su_stale) su_compute(); break;` (line ~10717)
- Cell color: after sg2_mode block (line ~12725)
- Sidebar: after sg2_mode panel (line ~19353)
- Keyboard: `key == 21` (Ctrl-U) after sg2 handler (line ~22690)
- Panel stacking: added to percolation panel (line ~19563)

## Visualization

- **Heatmap**: Cold blue → teal → yellow → orange → white-hot
- **Sidebar**: Global χ̄, peak χ location, Binder cumulant U₄, dual sparklines
- **Refresh**: Every generation (lazy recomputation on stale flag)

## Connections to other rounds

| Round | Overlay | Relationship |
|-------|---------|-------------|
| 73 | Spectral Gap | High χ ↔ small spectral gap (slow mixing at criticality) |
| 71 | Mean Field Deviation | High χ ↔ large MF deviation (correlations dominate) |
| 68 | Fisher Information | High χ ↔ high Fisher info (sensitivity at boundaries) |
| 58 | Early Warning | Variance-based warnings are susceptibility in disguise |
| 52 | Ising Hamiltonian | Energy fluctuations ∝ specific heat, dual to χ |
| 26 | Temperature Field | χ diverges at T_c |

## Explore / Exploit ratio
- **Exploit: 80%** — follows established overlay pattern exactly (state, compute, render, panel, keyboard)
- **Explore: 20%** — new physics: patch-based rolling variance, Binder cumulant U₄, adaptive normalization

## Technical details
- ~280 new lines in life.c
- Zero heap allocations (all static arrays)
- O(H×W) per frame compute cost
- Ring buffer for exact rolling window (no exponential smoothing approximation)
- Binder cumulant provides a parameter-free criticality diagnostic
