# Round 49: Percolation Analysis Overlay

## Goal
Add a percolation cluster analysis overlay that colors live cells by connected-cluster membership, highlights the largest cluster in gold, and detects spanning clusters (edge-to-edge connectivity). Bridges CA dynamics to percolation theory — a foundational concept in statistical physics.

## What was built

### Percolation Analysis Overlay (Toggle: `|`)
- **Cluster labeling**: 4-connected BFS flood fill assigns each live-cell cluster a unique ID (up to 2048 clusters)
- **Color scheme**: Largest cluster rendered in bright gold (255,200,40); all other clusters colored by golden-ratio hue with brightness proportional to relative size
- **Spanning detection**: Checks if the largest cluster touches both left+right edges (horizontal span) and both top+bottom edges (vertical span) — true percolation
- **Order parameter P∞**: Fraction of live cells in the largest cluster, the standard percolation order parameter
- **Density tracking**: Site occupation probability ρ displayed relative to the critical threshold p_c ≈ 0.593 for square lattice site percolation

### Sidebar Panel
- Row 1: Density ρ and cluster count (density color-coded relative to p_c)
- Row 2: Order parameter P∞ and largest cluster size
- Row 3: Spanning status with H/V indicators and "PERCOLATES" flag
- Row 4: Color legend (gold for largest + rainbow spectrum for others)
- Row 5: P∞ sparkline history (gold when high, dim when low)
- Row 6: Phase classification (Supercritical/Near-critical/Subcritical/Dilute)
- Row 7: Toggle hint with critical threshold reference

### Integration Points
- Full split-screen support (overlay index 21, name "Percolation")
- Auto-refresh every 2 generations when active
- Demo system support via `'|'` overlay character
- Reuses topo_qx/topo_qy BFS queue workspace (zero additional queue allocation)
- Proper stale flag management across grid updates

## Implementation details
- `perc_compute()`: Three-phase algorithm — (1) BFS cluster labeling, (2) largest cluster identification, (3) spanning edge-touch detection
- `perc_cluster_rgb()`: Gold for largest, HSV with size-scaled brightness for others
- 362 lines added (13,013 → 13,375)

## Explore/exploit balance
- **22% explore**: Introduces percolation theory — a new analytical dimension from statistical physics (critical phenomena, phase transitions, universality classes)
- **78% exploit**: Reuses BFS infrastructure from topological feature map, existing overlay pipeline, split-screen integration, sidebar panel template from ergodicity overlay

## Key insight
Conway's Life at typical random densities (~25%) is well below the site percolation threshold (~59.3%), producing fragmented clusters. But as patterns evolve, local density can exceed p_c in active regions — the overlay reveals where and when the system transitions from subcritical (isolated islands) to supercritical (connected network). This connects naturally to the correlation length and ergodicity overlays for multi-scale analysis.
