# Round 67: Cross-Correlation Matrix — Lead-Lag Relationship Discovery

## Goal
Add a Cross-Correlation Matrix overlay that reveals lead-lag relationships between the 16 global metric streams. The system tracks 16 scalar time series over a 128-frame window but had no way to see how metrics relate to each other — which ones lead, which lag, and which co-move.

## What was built
A new analytical overlay (toggle `2`, overlay #34) that computes pairwise time-lagged cross-correlation across all 16 metric streams, identifying causal lead-lag structure.

### Core algorithm: `xc_compute()` (~160 lines)
- **Cross-correlation at lags -8 to +8** for all 120 unique metric pairs (16 choose 2)
- **Optimal lag detection**: finds the lag maximizing |r| for each pair — positive lag means metric i leads metric j
- **Single-linkage clustering**: groups co-moving metrics using union-find with threshold |r| > 0.5
- **Per-cell projection**: correlates each cell's local 3×3 density history with global metrics, assigns the cell to the cluster of its best-matching metric

### Data pipeline
- `xc_record()`: samples all 16 metrics into a 128-frame ring buffer every 2 generations
- `xc_compute()`: triggered when stale and buffer has ≥20 samples
- Metrics sourced from existing `pp_read_metric()` infrastructure (population, entropy, Lyapunov, fractal dimension, flow, vortex count, wave energy, dS/dt, β₀, β₁, correlation length, Ising energy, magnetization, complexity, Kolmogorov, ergodicity)

## Sidebar panel (28 rows, teal theme)
1. **Window info** — sample count and mean |r| across all pairs
2. **16×16 mini heatmap** — color-coded by lead-lag direction:
   - Green: metric i leads metric j
   - Blue: metric j leads metric i
   - Red: synchronous (lag=0)
   - Intensity proportional to |r|
3. **Top-5 strongest pairs** — ranked by |r|, with direction arrows (→ for lead, ⇔ for sync), r-value, and lag
4. **Cluster info** — number of correlation clusters + color legend (lead/sync/lag)
5. **Sparkline** — 32-point history of mean |r| over time, teal gradient

## Per-cell visualization
5-zone HSV palette mapping cluster IDs to distinct hues:
- Zone 0 (0.0–0.2): cyan-blue
- Zone 1 (0.2–0.4): green-teal
- Zone 2 (0.4–0.6): amber-yellow
- Zone 3 (0.6–0.8): magenta-purple
- Zone 4 (0.8–1.0): warm red-orange

Alive cells brightened by +55 per channel.

## Technical details
- `XC_WINDOW=128`: sliding window size (matches existing metric buffers)
- `XC_MAX_LAG=8`: lag range tested in each direction
- `XC_HIST_LEN=64`: sparkline ring buffer
- `XC_TOP=5`: number of top pairs displayed
- State prefix: `xc_` (mode, buf, head, count, stale, peak_r, peak_lag, mean_abs_r, cluster, n_clusters, hist, grid)
- Minimum 8 overlapping samples required per lag computation
- Minimum 20 total samples before first computation

## Integration
- Split-screen overlay index 34, name "CrossCorr", key '2'
- Full sidebar stacking with all 33 prior overlays (11 downstream panels updated)
- `demo_reset_overlays()` clears xc_mode
- `split_set_overlay()` / `split_detect_current()` wired for index 34
- Status bar indicator: teal ▦XCOR with sample counter
- Ecosystem mode guard (disabled during dual-species mode)
- ~500 lines added to life.c

## Explore/exploit balance
- **Explore (10%)**: Time-lagged cross-correlation is a new analytical primitive not used elsewhere in the system
- **Exploit (90%)**: Reuses metric streams, 128-frame timeline buffer, overlay pattern, sidebar rendering, split-screen infrastructure, per-cell coloring pattern
