# Round 68: Fisher Information Field — Phase Boundary Sensitivity

## Goal
Add a Fisher Information overlay that computes per-cell sensitivity to perturbation, revealing where the system sits on its parameter landscape relative to phase transitions. Fisher information measures how sharply a probability distribution changes with respect to a control parameter — near critical points it diverges, making it the information-geometric dual of correlation length divergence.

## What was built
A new analytical overlay (toggle `1`, ghost layer 35) that estimates Fisher information at both the metric level and per-cell level, with critical boundary detection and Cramér-Rao duality visualization.

### Core algorithm: `fi_compute()` (~150 lines)

**Part 1 — Per-metric Fisher information:**
- Splits the time series buffer into two halves (early vs. recent)
- Builds 16-bin histograms for each half
- Computes Fisher information as Σ (Δp)²/p — how much the distribution shifted between the two epochs
- Identifies the most sensitive metric (highest Fisher info)

**Part 2 — Per-cell Fisher information from timeline:**
- For each cell, extracts 5×5 neighborhood density over the last 64 timeline frames
- Computes Fisher info from density dynamics: variance of density changes normalized by mean density × (1 - mean density)
- Adds second-order curvature term (variance of acceleration)
- Applies sigmoid compression to [0,1] for visualization
- Detects critical boundary cells (Fisher info > 0.6)
- Computes entropy curvature (-d²S/dρ²) for the Cramér-Rao duality panel

### Data pipeline
- `fi_record()`: samples all 16 metrics into a 128-frame ring buffer every 2 generations
- `fi_compute()`: triggered when stale and buffer has ≥16 samples
- Per-cell computation uses the timeline buffer (up to 64 frames of grid history)

## Sidebar panel (11 rows, violet theme)
1. **Global Fisher info** — I=value + state classification (STABLE/MODERATE/SENSITIVE/CRITICAL)
2. **Peak cell + boundary fraction** — highest per-cell Fisher info and percentage of cells above critical threshold
3. **Cramér-Rao duality** — lower bound on estimator variance (1/I) and entropy curvature (-d²S/dρ²)
4. **Top-4 sensitive metrics** — bar chart showing which of the 16 metrics have the highest Fisher information
5. **Color gradient legend** — deep blue → violet → magenta → gold → white-hot
6. **Scale labels** — stable / sensitive / critical
7. **Sparkline** — 32-point history of global Fisher info, violet-to-gold gradient

## Cell coloring (deep indigo → violet → magenta → gold → white)
- Low Fisher (stable): deep indigo-black
- Medium (sensitive): violet to magenta-pink
- High (near transition): magenta to warm gold
- Critical (phase boundary): gold to white-hot glow
- Creates a visually stunning "glow at the edge of chaos" effect

## Integration points
- **Key:** `1` (was unused)
- **State:** `fi_mode`, `fi_stale`, `fi_grid[MAX_H][MAX_W]`
- **Stale flag:** set in `grid_step()` after each generation
- **Status bar:** ◈FI:value indicator
- **Split-screen:** included in `split_set_overlay()` reset
- **Ghost layer:** 35

## Mathematical foundations
- **Fisher information:** I(θ) = E[(∂/∂θ log p(x|θ))²] — measures sensitivity of a statistical model to its parameter
- **Cramér-Rao bound:** Var(θ̂) ≥ 1/I(θ) — minimum variance of any unbiased estimator
- **Critical divergence:** near phase transitions, Fisher info diverges as I ~ |T - Tc|^(-α)
- **Entropy duality:** Fisher info equals negative curvature of entropy: I = -d²S/dρ²
