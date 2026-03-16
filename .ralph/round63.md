# Round 63: Causal Emergence Overlay (Effective Information)

## Goal
Add a "Causal Emergence" overlay that measures Effective Information (EI) at multiple coarse-graining scales to reveal where macro-level dynamics are more causally powerful than micro-level ones, based on Erik Hoel's causal emergence framework.

## What was built
A new grid-level analysis overlay (toggle `)`) that implements multi-scale causal emergence analysis:

- **Multi-scale coarse-graining**: Analyzes the grid at 4 scales — 1×1 (micro), 2×2, 4×4, and 8×8 blocks
- **Transition probability matrices**: For each block at each scale, extracts macro-state transitions from the last 32 timeline frames, quantizing alive-count into 16 bins
- **Effective Information (EI)**: Computes `EI = determinism - degeneracy` where:
  - Determinism = `log₂(n) - weighted avg row entropy` (how precisely causes map to effects)
  - Degeneracy = `H(column marginal)` (how uniformly effect states are visited)
- **Per-cell peak scale**: Each cell colored by which scale maximizes its EI
- **Sidebar panel**: Global EI, emergence fraction, peak scale, per-scale EI bar chart, color legend with scale labels, sparkline history, depth info

## Color scheme
- **Cool blue** — micro-level (1×1) is most causally powerful (no emergence)
- **Green** — 2×2 coarse-graining increases causal power
- **Warm orange** — 4×4 emergence
- **Bright red/white** — 8×8 macro-level dominance (strong emergence)
- Brightness modulated by EI magnitude

## Technical details
- `CE_DEPTH=32` timeline frames for transition estimation
- `CE_NSCALE=3` coarse-graining scales (2×2, 4×4, 8×8) plus micro baseline
- `CE_BLOCK_STATES=16` distinct macro-states per block (4-bit quantization)
- `CE_HIST_LEN=64` sparkline history samples
- EI clamped to non-negative values (negative EI → 0)
- Emergence threshold: cells with best_s > 0 and EI > 0.01 counted as emergent
- Auto-refreshes every 4 generations (uses timeline history)
- Stale-flag lazy computation for efficiency

## Integration points
- Split overlay table entry 29 ("CausalEmrg")
- Key: `)` (right parenthesis)
- Status bar indicator: `▦ε:pk=N×N` showing peak scale
- Panel stacking: positioned between coherence and percolation panels
- Demo scene support
- Also adds PowerSpec entry at slot 30 (key `6`) to overlay table

## Why this matters
Extends the R60-62 meta-analysis arc from "how do metrics relate?" (R60 correlation matrix) through "what frequencies dominate?" (R61 power spectrum) and "where is synchronization?" (R62 coherence) to "at what scale does the automaton's causal structure live?" — a fundamentally new analytical dimension. Regions where coarse-graining increases causal power represent true emergence in the formal information-theoretic sense.
