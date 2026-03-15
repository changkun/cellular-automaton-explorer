# Round 30 — Box-Counting Fractal Dimension Analyzer

## Goal
Add real-time fractal dimension estimation via the box-counting method, completing the scientific analysis suite (entropy → Lyapunov → Fourier → fractal dimension).

## What was built
- **Toggle**: `F` key activates/deactivates the fractal dimension overlay
- **Box-counting algorithm**: Covers the grid with boxes of ε = 2, 4, 8, 16, 32, 64, 128 cells and counts N(ε) occupied boxes at each scale
- **D_box estimation**: Linear regression on log(N) vs log(ε); fractal dimension = negative slope
- **R² goodness of fit**: Shows how well the power-law relationship holds
- **Per-cell overlay**: Colors cells by structural detail — cells in partially-filled coarse boxes (fractal boundaries) glow brighter than cells in fully-filled regions
- **Color palette**: Deep purple (coarse) → teal (mid) → gold (fine detail) → white (peak)
- **Overlay panel** (right side, stacks below other panels):
  - D_box value, R², alive cell count
  - Qualitative interpretation (sparse / line-like / filaments / moderate fractal / rich fractal / space-filling)
  - 4-row log-log scatter plot with regression line (teal) and data points (gold ●)
  - Scale legend (ε values) and N(ε) counts per scale
- **Status bar**: Shows `D₀=X.XXXX` in gold when active
- **Auto-refresh**: Recomputes every 8 generations while running

## Key design decisions
- 7 scales (ε = 2 to 128) span from fine-grained to coarse structure detection
- Per-cell detail metric uses scale isolation: cells in partially filled boxes at coarse scales score highest (they contribute to fractal boundary complexity)
- Lightweight compared to Fourier/Lyapunov — no shadow grids or FFT needed, just counting
- Interpretation thresholds: D < 0.5 sparse, 0.5–1.0 line-like, 1.0–1.3 thin fractal, 1.3–1.6 moderate, 1.6–1.8 rich, >1.8 space-filling

## Files modified
- `life.c`: ~200 lines added (global state, fractal_compute(), fractal_to_rgb(), cell overlay, panel overlay, key handler, status bar, auto-refresh)
