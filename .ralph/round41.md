# Round 41: Cell Probe Inspector

## Goal
Add a click-to-inspect tool that displays all 14+ analysis metrics for any single cell simultaneously, unifying the project's many independent overlays into one diagnostic panel.

## Motivation
The project has accumulated 14 independent scientific overlays across 15 rounds of development — entropy, temperature, Lyapunov, Fourier, fractal dimension, surprisal, mutual information, Kolmogorov complexity, composite complexity, frequency, information flow, RG scale, topology, and causal cones. Each computes per-cell values into dedicated grids, but users must toggle overlays one at a time to compare metrics. There is no way to see all measurements for a single cell at once. This feature is pure UI integration over existing computation — zero new algorithms, maximum leverage of existing analysis infrastructure.

## Implementation
- **Key:** `?` toggles probe mode on/off
- **Mouse interaction:** Left-click selects/re-targets a cell; right-click dismisses the panel
- **State machine:** `probe_mode` (0=off, 1=awaiting click, 2=showing panel), `probe_x`/`probe_y` for selected cell
- **Overlay panel:** Left-side, 48 chars wide, teal-themed (dark background #061006, teal borders/labels #00DCB4)
- **Metrics displayed (20 rows):**
  - Cell state (alive/dead), age (0–255), species (A/B in ecosystem mode)
  - Zone rule index and name
  - 3×3 Moore neighborhood mini-grid with alive count
  - Entropy (Shannon, color-coded by threshold)
  - Temperature (local + global)
  - Lyapunov exponent with stable/critical/chaotic label
  - Fourier spectral power
  - Fractal dimension with fine/meso/coarse label
  - Surprisal in bits
  - Mutual information coupling
  - Kolmogorov complexity with simple/structured/incompressible label
  - Composite complexity with dead/periodic/edge-of-chaos/chaotic classification
  - Oscillation frequency (dead/still/period-N/chaotic)
  - Information flow magnitude with directional arrow
  - RG dominant scale + scale invariance
  - Topology component ID, size, hole detection
  - Causal cone backward/forward depth
  - Ghost trail + tracer signal accumulation
- **Status bar:** `◆PROBE:click` or `◆PROBE:(x,y)` indicator; `[?]probe` in help line

## Architecture
No new computation or data structures beyond three state variables. Reads directly from all existing per-cell grids: `entropy_grid`, `temp_grid`, `lyapunov_grid`, `fourier_grid`, `fractal_grid`, `surp_grid`, `mi_overlay`, `kc_grid`, `cplx_grid`, `freq_grid`, `flow_mag`/`flow_vx`/`flow_vy`, `rg_grid`/`rg_invariance`, `topo_label`/`topo_hole`/`topo_comp_size`, `cone_back`/`cone_fwd`, `ghost`, `tracer`. Uses helper macros (`PROBE_ROW_START`, `PROBE_ROW_END`, `PROBE_PAD`) scoped within the rendering block.

## Lines added: +420 (10,259 → 10,675)

## Exploration vs Exploitation
95% exploitation — this adds no new analysis algorithms, instead integrating all existing overlays into a unified inspection tool. A force-multiplier for the analytical infrastructure built over the past 15 rounds.
