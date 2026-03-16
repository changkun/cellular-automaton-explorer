# Round 43: Spatial Correlation Length

## Goal
Add a spatial correlation length overlay (`&` key) that computes the two-point correlation function C(r) and extracts the characteristic correlation length ξ — a fundamental statistical physics observable missing from the analysis suite.

## Motivation
The project has entropy (local disorder), Fourier (frequency), fractal dimension (self-similarity), Lyapunov (sensitivity), and Kolmogorov (algorithmic complexity), but no direct measure of spatial correlation scale. Correlation length ξ is a fundamental quantity in statistical physics that reveals whether the automaton is in an ordered, critical, or disordered phase. It naturally complements the Wolfram classifier and composite complexity index. At round 43 with explore=39%, this is a pure exploration feature (new analytical dimension).

## Implementation

### Data Structures
- `corr_grid[MAX_H][MAX_W]` — per-cell local correlation strength 0.0–1.0
- `corr_cr[CORR_MAX_R+1]` — global C(r) function for r=0..32
- `corr_xi` — fitted correlation length ξ
- `corr_xi_r2` — R² goodness of exponential fit
- `corr_c1` — nearest-neighbor correlation C(1)
- `corr_density` — live cell density ρ
- `corr_hist_xi[64]` — sparkline history of ξ over time

### Algorithm
1. **Global density** ρ = count alive / total cells
2. **Two-point correlation** C(r) = ⟨s(x)·s(x+r)⟩ - ρ² sampled along 4 directions (E, SE, S, SW) for r=0..32
3. **Normalize** C(r) by C(0) (= variance σ²)
4. **Exponential fit** via linear regression on ln(C(r)) vs r for C(r) > 0.01: slope = -1/ξ
5. **Per-cell local correlation** — for each alive/ghost cell, sample 8 directions at distances 1..8, compute normalized covariance
6. **Phase classification** based on ξ: disordered (<2), short-range (2-5), near-critical (5-15), long-range order (>15)

### Color Scheme
- Violet (40,20,120) → Blue (60,80,200) → Cyan (20,220,230) → White (255,255,255)
- Uncorrelated → moderately correlated → strongly correlated

### UI Elements
- Stats panel (8 rows): ξ with R², C(1) and density, phase classification, C(r) sparkline (24 bins), ξ history sparkline, toggle hint
- Status bar: `◆ξ=X.X`
- Probe inspector: correlation value + global ξ
- Demo scene support: `&` character in overlay strings

### Integration Points
- Forward declaration: `corr_compute()`
- Stale marking in `grid_step()`
- Auto-refresh every 4 generations
- Demo overlay reset
- Keyboard toggle: `&`

## Lines Added
+401 lines (10,876 → 11,277)

## Classification
~90% exploration (new analytical dimension from statistical physics)
