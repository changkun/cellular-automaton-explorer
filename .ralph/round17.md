# Round 17: Pattern Stamp Tool with Rotatable Library

## Goal
Add a stamp mode with a built-in library of 20 classic Game of Life patterns that can be rotated and placed at any grid position. Enables compositional experimentation with precise known structures.

## What was built

### Pattern Library (20 patterns across 5 categories)
- **Still lifes**: Block, Beehive, Loaf, Boat, Tub
- **Oscillators**: Blinker, Toad, Beacon, Pentadecathlon, Clock
- **Spaceships**: Glider, LWSS, MWSS, HWSS
- **Methuselahs**: R-pentomino, Diehard, Acorn, Pi-heptomino
- **Other**: Gosper Gun, Pulsar

### Features
- **`S` key** toggles stamp mode (mutually exclusive with zone/emit/portal modes)
- **`[`/`]`** cycle through the 20 patterns while in stamp mode
- **Scroll wheel** rotates the selected pattern (0/90/180/270 degrees)
- **Left-click** places the pattern at cursor position (respects kaleidoscope symmetry)
- **Right-click** exits stamp mode
- **Preview overlay** in bottom-right corner shows current pattern name, rotation, and a visual preview of the cell layout
- **Status bar indicator** shows `▣STAMP:PatternName rotation°` in gold

### Technical details
- All rotations pre-computed at init time via rotation matrix around bounding box center
- Patterns stored as relative Offset coordinates, same type used by existing pattern system
- Placement uses existing `grid_set` / `sym_apply` for full integration with ecosystem species, symmetry modes
- Flash message confirms placement with pattern name and rotation

## Interactions with existing features
- **Kaleidoscope symmetry**: Stamps placed with 2/4/8-fold symmetry for mandala patterns
- **Ecosystem mode**: Stamps placed as the active brush species (A or B)
- **Zone boundaries**: Place patterns precisely at zone edges to study cross-rule interactions
- **Wormhole portals**: Place gliders aimed at portal entrances for tunneling experiments
- **Signal tracer**: Track stamp-originated structure flow paths
