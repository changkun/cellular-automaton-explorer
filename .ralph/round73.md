# Round 73 — Spectral Gap Estimator

## Goal
Add a Spectral Gap Estimator overlay that computes the local Markov
mixing rate per cell — the canonical quantity unifying relaxation time,
correlation decay, and critical slowing down into a single number.

## What was built
- **Overlay #39** (ghost layer 40) — Toggle key `Ctrl-T`
- Per-cell 2×2 Markov transition matrix tracking: exponential-smoothed
  counts of dead→dead, dead→alive, alive→dead, alive→alive transitions
- Computes the second eigenvalue λ₂ = 1 - P(dead→alive) - P(alive→dead)
  of the local transition matrix
- Spectral gap = 1 - |λ₂| ∈ [0,1]: small gap = slow mixing (near
  criticality), large gap = fast equilibration

## Visualization
- **Heatmap**: indigo/violet (gap≈0, critical/slow) → teal (moderate) →
  green-lime (healthy mixing) → yellow (fast) → white-yellow (instant)
- **Sidebar panel** with:
  - Mean spectral gap with interpretive classification
    (CRITICAL / SLOW / MODERATE / FAST / INSTANT)
  - Min–max range showing gap distribution extremes
  - Critical cell fraction (gap < 0.1) and fast cell fraction (gap > 0.8)
  - Color scale legend
  - Dual sparklines: mean gap and critical fraction over time

## Algorithm
1. Each step, record whether each cell was alive or dead in previous state
2. After CA step, for each cell: exponential-smooth (α=0.98) the four
   transition counts (dd, da, ad, aa)
3. Compute transition probabilities: P(dead→alive) = da/(dd+da),
   P(alive→dead) = ad/(ad+aa)
4. For a 2×2 stochastic matrix, λ₂ = 1 - P(da) - P(ad) analytically
5. Spectral gap = 1 - |λ₂|, clamped to [0,1]
6. Aggregate global statistics: mean, min, max, critical/fast counts

## Technical details
- ~200 lines added to declarations + compute + rendering + sidebar + key handler
- Zero dynamic allocation, all static arrays
- Exponential smoothing (α=0.98) provides adaptive memory that tracks
  rule mutations and phase transitions naturally
- Lazy recomputation via sg2_stale flag
- Integrated into split-screen system as overlay index 39
- Integration points: overlay table, split_set_overlay,
  split_detect_current, split_ensure_computed, cell_color (ghost code
  40), grid_step stale flag, keyboard handler, demo_reset_overlays
- Named sg2_ internally to avoid collision with sg_ (Symmetry Group)

## Explore/Exploit: 5% explore / 95% exploit
- Exploits the identical infrastructure pattern established by 38 prior
  overlays — same variable layout, stale flag, compute function,
  cell_color block, sidebar panel, key handler, split-screen integration
- The spectral gap is a well-known quantity from Markov chain theory,
  not a novel concept — but it had not been computed for this system
- Directly exploits Round 72's transition observation framework: the
  per-cell birth/survival counting scheme pioneered by Rule Inference
  is generalized here into a full transition matrix
- Exploits the exponential smoothing pattern used by Mean Field (R71)

## Connections
- **Unifying quantity**: the spectral gap is the single number that
  connects Early Warning (R31, critical slowing down = gap→0),
  Correlation Length (R14, diverges as gap→0), Mean Field Deviation
  (R71, non-MF regions often have small gaps), and Fisher Information
  (R68, peak sensitivity at small gaps)
- Complementary to Ergodicity overlay (R18): ergodic regions should
  have large spectral gaps; non-ergodic regions should have small ones
- The Lyapunov exponent (R3) measures sensitivity to perturbation in
  state space; the spectral gap measures rate of information loss —
  these are dual perspectives on the same dynamics
- Zone boundaries (visible in Rule Inference R72) should show up as
  sharp spectral gap transitions
