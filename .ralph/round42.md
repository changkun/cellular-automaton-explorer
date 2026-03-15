# Round 42: Auto-Demo Mode

## Goal
Add an auto-demo mode (`D` key) that runs a self-guided tour through curated pattern + overlay combinations, making 41 rounds of features discoverable in a 2-minute showcase.

## Motivation
The project has accumulated 20 scientific overlays, 10 rulesets, and 5+ preset patterns across 41 rounds, but discovering the best combinations requires deep familiarity with every keybinding. New users see a blank grid and a wall of help text. An auto-demo mode turns the power-user research tool into something anyone can appreciate on first launch — zero new algorithms, pure exploitation of existing features.

## Implementation

### Data Structure
- `DemoScene` struct: pattern ID, ruleset index, overlay toggle string, caption text
- 10 curated scenes stored in `demo_scenes[]` static array
- State tracked via `demo_mode`, `demo_scene`, `demo_scene_gen`, `demo_fade`, `demo_caption`

### Scene Definitions (10 scenes)
1. **Gosper Gun + Information Flow** (`O`) — causal influence radiates with gliders
2. **R-pentomino + Lyapunov Sensitivity** (`L`) — chaos mapped at expanding frontier
3. **Diamoeba (random) + Fractal Dimension** (`F`) — organic boundaries measured
4. **Acorn + Entropy** (`i`) — Shannon entropy localizes active computation
5. **Gosper Gun + Frequency Analysis** (`f`) — periodic structures revealed
6. **Day & Night (random) + Complexity Index** (`#`) — edge-of-chaos dynamics
7. **R-pentomino + Prediction Surprise** (`!`) — emergent novelty highlighted
8. **HighLife (random) + Topological Features** (`$`) — components and holes tracked
9. **Maze (random) + Kolmogorov Complexity** (`^`) — algorithmic depth of corridors
10. **Acorn + Renormalization Group + Heatmap** (`%h`) — multi-scale structure

### Key Functions
- `demo_reset_overlays()` — clears all 25+ overlay/mode flags between scenes
- `demo_setup_scene(idx)` — loads pattern, sets ruleset, enables overlays, centers viewport
- `demo_fade_apply(RGB*)` — dims cell colors over 12 frames for smooth transitions

### UI Integration
- Status bar shows `▶ DEMO` indicator in magenta when active
- Help bar replaced with caption: scene counter, description, progress percentage
- Caption text fades along with cells during transitions
- `[D]demo` added to normal help bar

### Controls
- `D` (uppercase) starts/stops demo mode
- Any key during demo exits and keeps current scene for exploration
- `d` (lowercase) still toggles draw mode as before
- Demo runs at 60ms speed, 150 generations per scene, 12-frame fade transition

### Architecture Notes
- Forward-declares 14 overlay compute functions to call them from `demo_setup_scene`
- Overlay toggle chars in scene definitions match the actual keyboard shortcuts
- Multi-overlay scenes use concatenated chars (e.g., `"%h"` for RG + heatmap)
- Fade applied in render loop via `demo_fade_apply()` on both full-cell and half-block paths
- Scene advancement happens in the simulation tick block, after `grid_step()`
