# Round 65: Hurst Exponent / Long-Range Memory Overlay

## Goal
Add a Hurst exponent overlay that measures long-range dependence via Rescaled Range (R/S) analysis, revealing whether the automaton's dynamics have memory — persistent trends, pure randomness, or anti-persistent mean-reversion.

## What was built
A new analysis overlay (toggle `4`, overlay #32) implementing R/S analysis on both global metric streams and per-cell density history:

- **Rescaled Range (R/S) analysis**: For each of the 16 scalar metrics, computes R/S statistics at multiple window sizes (8, 16, 32, 64 frames), then estimates H from the log-log slope via linear regression
- **Per-cell Hurst**: Extracts 64-frame binary cell state history from the timeline buffer, computes R/S at scales 4, 8, 16, 32
- **Classification**: PERSISTENT (H>0.65), trending (H>0.55), BROWNIAN (0.45-0.55), reverting (H<0.45), ANTI-PERSISTENT (H<0.35)

## Color scheme
- **Bright cyan/blue** — H > 0.7, strongly persistent (trending regions)
- **Mild cyan** — H 0.5–0.7, weakly persistent
- **Gray** — H ≈ 0.5, Brownian / no memory
- **Orange** — H 0.3–0.5, weakly anti-persistent
- **Bright orange-red** — H < 0.3, strongly anti-persistent (mean-reverting)
- Alive cells brightened by +55 per channel

## Sidebar panel (9 rows, teal theme)
1. Global H̄ with classification label + sample count
2. Most deviant metric (highest |H - 0.5|) with H value
3. Bar chart of top 4 most deviant metrics (cyan=persistent, orange=anti-persistent, gray=Brownian)
4. Color gradient legend (40-character continuous spectrum)
5. Scale labels: anti-persist ← H=0.5 → persistent
6. Sparkline of global H over time (color-coded)
7. Memory interpretation text (plain-language description)
8. Bottom border

## Technical details
- `HR_WINDOW=128` sliding window for metric R/S analysis
- `HR_N=16` metrics (same as PP_N_METRICS)
- `HR_HIST_LEN=64` sparkline history
- R/S computed via `hr_rs_single()`: mean → cumulative deviations → range R → std dev S → R/S
- Hurst estimated by log-log linear regression of R/S vs window size
- Per-cell uses 4 scales (4, 8, 16, 32) on 64 timeline frames
- Needs ≥32 samples before producing estimates (defaults to H=0.5)
- Records every 2 generations (same cadence as other metric overlays)

## Integration
- Split-screen overlay index 32, name "HurstExp", key '4'
- Full sidebar stacking with all 31 prior overlays
- Ecosystem mode guard (disabled during dual-species mode)
- ~522 lines added (19,256 total)

## Explore/exploit balance
- **Explore (~40%)**: Rescaled Range analysis and Hurst exponent are entirely new concepts not present in the system — a new statistical framework from fractal time series analysis
- **Exploit (~60%)**: Reuses the 16-metric infrastructure, 256-frame timeline buffer, ring buffer pattern, sidebar rendering template, and split-screen system
