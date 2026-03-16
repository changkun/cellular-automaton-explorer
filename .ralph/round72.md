# Round 72 — Rule Inference Engine

## Goal
Add a Rule Inference Engine overlay that watches the CA's dynamics and
reconstructs the birth/survival rules from observation alone — the
simulation performing empirical science on its own behavior.

## What was built
- **Overlay #38** (ghost layer) — Toggle key `Ctrl-R`
- Per-cell transition tracking: for each neighbor count k (0-8), records
  birth events (dead→alive) and survival events (alive→alive)
- Accumulates histograms `ri_cell_birth_yes/no[y][x][k]` and
  `ri_cell_surv_yes/no[y][x][k]` plus global aggregates
- Computes empirical P(birth|k) and P(survive|k), infers B/S bitmasks
  when probability > 0.5
- Zone-aware comparison of inferred rules against actual active ruleset
  → per-cell **fidelity map**
- Periodic 3/4 decay on histograms to adapt to rule mutations over time

## Visualization
- **Fidelity map**: green (rules confirmed) → yellow (slight deviation
  from temperature/noise) → orange (zone boundary effects) → red
  (different rule region detected)
- **Sidebar panel** with:
  - Inferred B/S string vs actual, colored by match status
  - Global fidelity percentage
  - Per-bit probability bars for birth (B:) and survival (S:) — height
    encodes probability, color encodes match/mismatch
  - Sparklines for fidelity and sample count over time
  - Sample counter and color legend

## Algorithm
1. Each step, record previous state and neighbor count for every cell
2. After CA step, for each cell: increment birth_yes/no or surv_yes/no
   histogram bin for the observed (state, k, outcome) triple
3. Compute P(birth|k) = birth_yes[k] / (birth_yes[k] + birth_no[k])
   and similarly for survival
4. Infer B/S rule: bit k is set if P > 0.5
5. Compare inferred rule against zone-specific actual rule → fidelity
6. Apply 3/4 decay periodically to let statistics adapt to mutations

## Technical details
- ~593 lines added (~22,250 → ~22,840)
- Zero dynamic allocation, all static arrays
- Lazy recomputation via ri_stale flag
- Integrated into split-screen system as overlay index 38
- Integration points: overlay table, split_set_overlay,
  split_detect_current, split_ensure_computed, cell_color (ghost code
  39), grid_step stale flag, keyboard handler

## Explore/Exploit: 90% explore / 10% exploit
- Entirely new analytical category: epistemology / self-referential
  inference, distinct from all prior overlays (dynamics, topology,
  thermodynamics)
- Empirical rule reconstruction from observation is a novel concept for
  the project
- Exploits existing overlay infrastructure, zone system, and sidebar
  panel pattern

## Connections
- **Meta-scientific loop**: the simulation does empirical science on its
  own dynamics, reconstructing rules from observed transitions
- Ties together multi-rule zones (R8), mutations (R4), and the Wolfram
  classifier (R31) into a unified inferential framework
- Fidelity map naturally reveals zone boundaries where mixed statistics
  produce low confidence
- Validates that zones and mutations are behaving as intended; detects
  edge effects at boundaries
