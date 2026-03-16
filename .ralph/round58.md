# Round 58: Particle Tracker Overlay

## Goal
Add a particle tracker overlay that auto-detects moving structures (gliders, spaceships) by matching connected components across frames, computing velocity vectors, and rendering a real-time velocity field with speed coloring and motion statistics.

## What was built
- **Particle Tracker overlay** (toggle `7`): 28th analysis overlay
- **Connected component tracking**: 8-connected BFS labels all live cell clusters each frame
- **Cross-frame matching**: Nearest-centroid matching with size-ratio penalty links components between generations
- **Velocity field**: Per-cell velocity vectors (vx, vy) and speed painted onto the grid
- **Speed coloring**: Blue=still → cyan=slow → green=medium → yellow=fast → red=very fast
- **Directional alignment brightening**: Cells moving with the dominant flow get extra brightness
- **Statistics sidebar panel**:
  - Total components, moving/still counts
  - Collision events (merge/split detection)
  - Max and mean speed
  - Dominant direction with arrow indicator
  - Moving-particle-count sparkline (64-frame history)
- **Split-screen support**: Added as overlay index 27 in the dual-panel system
- **Auto-demo support**: Can be activated via demo scene configuration

## Technical details
- `PT_MAX_PARTICLES = 512` tracked components per frame
- Match threshold: 8-cell max centroid displacement, 3x size ratio limit
- Speed threshold for "moving": 0.3 cells/generation
- Recomputes every 2 generations when active
- ~200 lines of new code

## Key insight
This is the first overlay that performs **kinematic analysis** — tracking *who moves where* rather than *what exists*. Previous overlays measured topology, entropy, and dynamics but couldn't identify individual structures or their trajectories. This bridges structural and kinematic understanding of the automaton.
