# Round 4: Live Rule Mutation — B/S Rule Space Explorer

## Goal
Transform the fixed Conway's Life simulator into a general Life-like cellular
automaton explorer. Users can cycle through 10 preset rulesets or mutate rules
in real-time, watching the population react as the physics change beneath them.

## What was built
- **Bitmask-based rule engine**: Birth and survival conditions stored as
  `unsigned short` bitmasks (bit N = neighbor count N is active). Replaced the
  hardcoded `n == 3 || (n == 2 && grid[y][x])` with generic bitmask checks.

- **10 preset rulesets**: Conway, HighLife, Day & Night, Seeds, Diamoeba,
  Morley, 2x2, Maze, Coral, Anneal — each with dramatically different dynamics.

- **Rule cycling** (`[` / `]`): Step through presets, instantly applying new
  rules to the existing grid state.

- **Mutation** (`m`): Randomly flips one bit in either the birth or survival
  mask, creating hybrid "mutant" rules. Status bar shows `(mutant)` when the
  active rule doesn't match any named preset.

- **Status bar integration**: Shows current rule in standard `B3/S23` notation
  with the preset name (or `mutant` label) in the status bar.

## Key design decisions
- Rules encoded as bitmasks for O(1) lookup — no arrays or loops needed at
  the hot path in `grid_step()`.
- Mutation flips exactly one bit (out of 18 possible: 9 birth + 9 survival)
  to keep changes small enough to observe incremental behavioral shifts.
- Existing patterns (glider, gun, etc.) are Conway-specific but still serve
  as interesting initial conditions for other rulesets.

## Files changed
- `life.c` — rule system, key handlers, status bar, help text
- `README.md` — updated title, description, controls, rule preset table
