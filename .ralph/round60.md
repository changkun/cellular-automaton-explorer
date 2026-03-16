# Round 60: Metric Correlation Matrix Enhancement

## Goal
Enhance the existing correlation matrix overlay to use a 128-frame sliding window and show top 3 strongest correlations + top 3 strongest anti-correlations.

## What was built
The correlation matrix overlay already existed (toggle `'`), added in a prior round. This round enhanced it:

- **Doubled sliding window**: 64 → 128 frames, matching the anomaly detector's window
- **Top 3 correlations**: Sidebar now shows the 3 strongest positive correlation pairs (was: only 1)
- **Top 3 anti-correlations**: Sidebar now shows the 3 strongest negative correlation pairs (was: only 1)
- **Removed "independent" pair**: Replaced with additional correlation/anti-correlation entries for more useful information density

## What already existed (from prior round)
- 16×16 Pearson correlation heatmap (red = +1, white = 0, blue = -1)
- Toggle with `'` key
- Abbreviated metric labels on axes
- Ring buffer recording every 2 generations via `cm_record()`
- Full Pearson computation via `cm_compute()` with proper mean/stddev/covariance
- Panel stacking with other overlays

## Technical details
- `CM_WINDOW` increased from 64 to 128
- Top-3 finding uses insertion sort into small arrays (O(n²) pairs × O(3) insertion = negligible)
- Panel height increased from `8 + CM_N` to `11 + CM_N` rows (3 extra summary rows)
- Updated all downstream panel stacking offsets (recurrence plot, 3D attractor, particle tracker)
- Added README documentation for the `'` key binding and feature description

## Design decisions
- **Kept existing key binding `'`**: The task suggested `)` but that's already taken by Phase Portrait. The overlay already has a natural key.
- **128-frame window**: Matches the anomaly detector's AD_WINDOW for consistency — both look at the same temporal scale.
- **Top 3 + top 3 instead of 1+1+independent**: Six ranked pairs give much richer relationship discovery than the old three.
