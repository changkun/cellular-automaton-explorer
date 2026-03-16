# Round 66: Regime Dashboard — Unified Status Synthesis Overlay

## Goal
Build a dashboard overlay that synthesizes signals from the last 6 meta-analytical overlays into a single unified status panel. With 32+ overlays and only 1-2 visible at a time, the user needs a single view aggregating regime classification, transition risk, memory character, spectral peaks, emergence scale, and anomaly alerts.

## What was built
A new synthesis overlay (toggle `5`, overlay #33) that reads from 6 existing subsystems without introducing new algorithms — pure aggregation:

- **Wolfram classifier** (overlay 19) → current regime class (I–IV) + confidence
- **Early warning signals** (overlay 31) → transition risk level + per-cell warning grid
- **Hurst exponent** (overlay 32) → memory character (persistent/Brownian/anti-persistent)
- **Power spectrum** (overlay 30) → dominant spectral period + source metric
- **Causal emergence** (overlay 29) → peak emergence scale + emergence index
- **Anomaly detector** → active alert count + top 2 alert details (metric + z-score)

## Sidebar panel (11 content rows, lavender theme)
1. **Regime** — Wolfram class (I-IV) with confidence, color-coded by class (blue/green/red/gold)
2. **Risk** — 20-char bar showing transition risk from early warning (LOW/MODER/HIGH/CRIT)
3. **Memory** — Hurst H value with classification (PERSISTENT/Brownian/ANTI-PERSIST)
4. **Freq** — Dominant spectral period with source metric name
5. **Emrg** — Peak causal emergence scale (1×1 to 8×8) with emergence index
6. **Anomal** — Active anomaly count with first 2 alert details (metric + z-score)
7. Separator
8. **Risk sparkline** — 32-point history, green→red gradient
9. **H̄ sparkline** — 32-point Hurst history, cyan/gray/orange gradient
10. **Color legend** — stable→active→critical gradient explanation
11. Bottom border

## Per-cell composite grid
Weighted heatmap blending three per-cell data sources:
- Early warning grid: 50% weight (transition risk dominates)
- Hurst deviation from 0.5: 30% weight (memory strength)
- Causal emergence scale: 20% weight (multi-scale emergence)

Color gradient: cool blue/green (stable) → warm orange/red (active/critical). Alive cells brightened by +55 per channel.

## Key behaviors
- Pressing `5` auto-activates EW and Hurst data feeds if not already running
- Recomputes every 4 generations
- Triggers Wolfram classification on first activation if not yet run
- Works in split-screen mode (overlay 33 in the table)
- 64-frame sparkline history ring buffer for risk and Hurst trends

## Technical details
- `RD_HIST_LEN=64` sparkline history buffer
- State variables: `rd_regime`, `rd_risk`, `rd_hurst`, `rd_peak_period`, `rd_emerg_scale`, `rd_emerg_idx`, `rd_n_anomalies`
- `rd_compute()`: reads from `wolfram_class`, `ew_global_warning`, `hr_global_h`, `ps_total_power[]`, `ps_peak_period[]`, `ce_peak_scale`, `ce_emergence_idx`, `ad_n_alerts`, `ad_alerts[]`
- `rd_grid[y][x]`: composite blend of `ew_grid`, `hr_grid`, `ce_grid` per cell
- Panel stacking integrated across all 10+ downstream overlay panels

## Integration
- Split-screen overlay index 33, name "Dashboard", key '5'
- Full sidebar stacking with all 32 prior overlays
- Ecosystem mode guard (disabled during dual-species mode)
- ~500 lines added

## Explore/exploit balance
- **Explore (0%)**: No new algorithms or analytical concepts
- **Exploit (100%)**: Pure synthesis layer — reads existing data structures from 6 subsystems, introduces no new computation, only aggregation and visualization
