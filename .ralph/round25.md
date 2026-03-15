# Round 25: Population Dynamics Dashboard

## Goal
Add a full population dynamics dashboard overlay that visualizes population trends over time with statistics, per-species breakdown, and a multi-row graph.

## What was built
- **Dashboard overlay** toggled with `y` key — a centered panel showing:
  - **Statistics row**: current population, delta (Δ), min, max, average over history
  - **Trend indicator**: growing/rising/stable/falling/dying with arrow icons, computed from 10-gen moving window
  - **Species breakdown**: when ecosystem mode is active, shows species A (blue) and B (red) populations
  - **16-row graph**: full Unicode block chart plotting population history (up to 120 generations), with Y-axis labels (min/mid/max), color-coded line (red→yellow→green by level)
  - **Ecosystem mode**: overlays species A (blue) and B (red) population curves on the same graph; overlapping points shown in white
  - **Legend row**: shows line colors, generation count, and growth rate

- **Per-species history tracking**: extended the existing `pop_history` ring buffer with parallel `pop_history_a` and `pop_history_b` arrays, counted during `hist_push()` when ecosystem mode is active

## Technical details
- Reuses the existing HIST_LEN=120 ring buffer infrastructure
- Dashboard rendered as ANSI overlay (like census/genetic explorer panels) using box-drawing characters
- Blue-tinted color scheme (border: cyan, background: dark navy) to differentiate from census (green) and genetic (gold) overlays
- Graph uses `░` for fill below the line, `█` for the line itself, `·` for empty columns
- Y-axis auto-scales to min/max of visible history

## Key bindings
- `y` / `Y` — Toggle population dynamics dashboard

## Files modified
- `life.c` — Added dashboard_mode, per-species history arrays, hist_get_a/hist_get_b helpers, full overlay rendering, key handler, help bar update
- `README.md` — Added `y` key documentation
