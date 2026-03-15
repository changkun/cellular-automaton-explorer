# Round 16: Dual-Species Ecosystem Mode

## Goal
Introduce a second cell species (Red vs Blue) coexisting on the same grid with independent birth/survival rules and a configurable inter-species interaction coefficient. This creates predator-prey, competition, and mutualistic dynamics — Lotka-Volterra oscillations, territorial boundaries, extinction cascades, and symbiotic structures that no single-species CA can produce.

## What was built
- **Species grid**: `species[MAX_H][MAX_W]` / `next_species[MAX_H][MAX_W]` double-buffered per-cell species identity (0=none, 1=A, 2=B)
- **Per-species rules**: Species A defaults to Conway (B3/S23), Species B to HighLife (B36/S23), stored as independent birth/survival bitmasks
- **Interaction coefficient**: float from -1.0 (hostile) to +1.0 (cooperative), adjustable with `{`/`}` keys
- **Species-aware neighbor counting** in `grid_step()`: counts same-species (`n_a`) and cross-species (`n_b`) neighbors separately, computes effective count as `same + interaction * cross`
- **Birth arbitration**: when both species qualify to birth into a dead cell, the species with more neighbors wins (random tiebreak)
- **Brush species toggle**: `6` key switches active drawing brush between species A and B
- **Species A rendering**: cool blue-cyan age-based gradient (deep blue → light cyan → near-white)
- **Species B rendering**: warm red-orange age-based gradient (deep red → amber → near-white warm)
- **Minimap integration**: blue dots for A, red for B, purple for mixed-species regions
- **Status bar**: shows active brush species, interaction coefficient with sign, colored by species
- **Timeline integration**: species grid stored per frame for time-travel rewind/forward
- **Save/load**: species data appended to `.life` binary format with backward compatibility (old files without ecosystem data load fine)
- **Drawing**: places cells of the active brush species; erase clears species identity
- **Randomize**: in ecosystem mode, live cells get randomly assigned 50/50 between species
- **Clear**: resets species grid along with main grid

## Key design decisions
- **Ecosystem bypasses zone rules**: combining per-species rules with per-cell zone rulesets would create combinatorial complexity (2 species × 10 zone rulesets = 20 rule contexts per cell). Instead, ecosystem mode uses its own per-species bitmasks directly, keeping the mental model simple.
- **Additive interaction model**: the effective neighbor count formula `same + interaction * cross` creates a smooth spectrum: at interaction=0 species ignore each other entirely; at +1.0 cross-species neighbors help equally (full mutualism); at -1.0 they count negatively (hostile competition causing overpopulation death).
- **Rounding with sign preservation**: effective neighbor counts are rounded to nearest integer with sign-aware rounding, then clamped to [0,15] to index the B/S bitmask safely.
- **Backward-compatible save format**: ecosystem data is appended after portal data; a single `fgetc` test at load time detects presence/absence.

## Lines changed
~280 lines added to life.c (2752 → ~3030)

## Interactions with existing systems
- **Timeline/replay**: species identity travels through time with the rest of the grid state
- **Save/load**: fully persisted including per-species rules and interaction coefficient
- **Drawing + symmetry**: brush species respected in all symmetry modes
- **Minimap**: species-aware coloring shows territorial structure at a glance
- **Heatmap**: ecosystem mode overrides heatmap coloring with species-specific gradients
- **Wormhole portals**: portal neighbor contributions are counted but not species-decomposed (they add to total `n` only) — an intentional simplification
- **Emitters**: fire cells of current brush species
