# Round 51: Hamiltonian Energy Landscape (Ising Model Analogy)

## Goal
Add an Ising-model energy overlay that maps alive/dead cells to spin ±1,
computes per-cell Hamiltonian energy H = -s·Σs_neighbors, and visualizes
the energy landscape revealing stable structures (energy wells) vs
frustrated boundaries (domain walls).

## What was built
- **Toggle**: `;` key activates the Ising Energy Landscape overlay
- **Physics**: Each cell treated as spin s_i = +1 (alive) or -1 (dead).
  Hamiltonian H_i = -s_i · Σ s_neighbors over Moore neighborhood (8 neighbors).
  Normalized to [-1, +1] range for visualization.
- **Color scheme**: Deep blue (energy well, H < 0, aligned neighbors) →
  cyan (shallow well) → yellow/orange (mild frustration) → bright red
  (strongly frustrated, H > 0)
- **Magnetization** m = mean spin across grid: +1 = all alive, -1 = all dead
- **Susceptibility** χ = variance of local magnetization (peaks at criticality)
- **Domain walls**: Count of misaligned neighbor pairs (phase boundaries)
- **Frustrated cells**: Count of cells with positive energy (locally unfavorable)
- **Sidebar panel**: Total H, m, χ, domain wall count + density %, color legend,
  energy sparkline (30 frames), phase classification
- **Phase classification**: Ordered (ferromagnetic), Partially ordered,
  Critical (high χ), Disordered (paramagnetic), Mixed phase

## Integration points
- Split overlay table: index 22, key `;`
- split_set_overlay, split_detect_current, split_ensure_computed
- split-screen save/restore variables
- demo_reset_overlays, demo_setup_scene
- Stale flag in update(), auto-refresh every 2 generations
- Cell color rendering with brightened alive cells
- Forward declaration for ising_compute()

## Insights
- Still lifes (blocks, beehives) sit in deep energy wells — all neighbors
  aligned → minimum H → explains their stability thermodynamically
- Oscillators cycle between energy states, visible as breathing blue/yellow
- Random initial conditions show paramagnetic disorder (low |m|, moderate χ)
- Conway's Life tends toward low-energy configurations over time as
  unstable structures decay — visible as the grid "cools" in the energy map
- Domain walls trace exact boundaries between alive/dead regions,
  highlighting the geometric structure of pattern boundaries
- Susceptibility χ spikes during phase transitions (randomization → settling)

## Type: Exploration (91%)
New analytical lens from statistical mechanics / condensed matter physics.
Complements existing thermodynamic overlays (entropy production, ergodicity,
temperature field) by providing the missing energy/stability perspective.

## Lines added: ~230
