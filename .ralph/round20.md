# Round 20: Genetic Rule Explorer

## Goal
Add an automated rule discovery engine that evolves Life-like rulesets toward "interesting" behavior using genetic algorithms, letting the simulation explore its own rule space.

## What was built
- **Genetic search engine**: Evaluates candidate B/S rulesets by running 200-step mini-simulations on an 80×60 test grid seeded with R-pentomino + random scatter
- **Multi-factor fitness scoring**: Population stability (density in 5-50% range), dynamism (coefficient of variation), oscillation detection (autocorrelation of late-phase population), longevity, and structural complexity (distinct population values)
- **Evolution operators**: Mutation (flip 1-3 random B/S bits), uniform crossover of B/S bitmasks, elitism (keep top 5 unchanged/slightly mutated)
- **Population management**: 20 candidates per generation — seeded from current rule mutations, known preset mutations, and random rules (gen 0); bred from elites with diversity injection (gen 1+)
- **Center-screen overlay**: Gold-bordered panel showing top 5 discovered rules with their fitness scores, generation counter, and interaction instructions
- **Key bindings**: `G` to start/breed next generation, `1-5` to load a rule, `q`/`ESC` to close overlay
- **Status bar indicator**: Shows `⁂EVOLVE...` during search, `⁂EVOLVE G{n}` when showing results

## Key design decisions
- **Mini-grid simulation** (80×60, 200 steps) rather than evaluating on the live grid — keeps search fast (~50ms per generation of 20 candidates) without disrupting the user's work
- **Scoring favors "interesting" over "alive"**: Rules that produce static fills or immediate extinction score 0; highest scores go to rules with moderate density, some population variance, and detectable periodicity (like Conway's B3/S23)
- **Gene overlay intercepts q/ESC/1-5**: When the overlay is visible, these keys control the explorer rather than quitting or loading preset patterns
- **Stateless evaluation**: Each candidate gets a fresh mini-grid with identical initial conditions (R-pentomino + random scatter), so scores are comparable within a generation

## Integration points
- Uses existing `birth_mask`/`survival_mask` globals and `find_matching_ruleset()` for seamless rule loading
- Flash message system for load confirmation
- Status bar indicator system (follows same pattern as census, freq, tracer indicators)
- Help bar updated with `[G]evolve`
- Overlay rendering follows census/frequency overlay box-drawing convention

## Technical notes
- 20 candidates × 200 steps × 80×60 grid = ~19M cell evaluations per search step — completes in well under 100ms on modern hardware
- `pop_history[]` initialized to zero to prevent UB on early termination
- Valid steps tracked correctly through early-exit breaks (extinction/explosion)
- No heap allocation — all state is static arrays
