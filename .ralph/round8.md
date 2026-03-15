# Round 8: Multi-Rule Zones

## Goal
Add spatial rule heterogeneity — allow different regions of the grid to run different Life-like rulesets simultaneously, creating emergent behavior at zone boundaries.

## What Was Built

### Multi-Rule Zone System
- **Zone grid**: `zone[MAX_H][MAX_W]` stores per-cell ruleset index (0-9)
- **Zone-paint mode** (`j` key): mouse painting assigns rulesets to grid regions
  - Left-click/drag paints current zone brush (3x3 brush for coverage)
  - Right-click/drag resets to default zone (Conway)
  - `[`/`]` cycles zone brush through all 10 rulesets
  - Works with kaleidoscope symmetry (2/4/8-fold symmetric zone painting)
- **Per-cell rule lookup**: `grid_step()` reads each cell's zone to determine birth/survival masks
- **Zone visualization**: color-coded background tinting when zone mode is active
  - 10 distinct zone colors (blue, purple, teal, orange, brown, rose, olive, sea green, gold, lavender)
  - Alive cells get 20% zone color blend
  - Empty cells show dim zone tint
  - Works across all 3 zoom levels (1x, 2x half-block, 4x quarter-block)

### Preset Zone Layouts (keys 1-5 in zone mode)
1. **Vertical split**: Conway left, HighLife right
2. **4 quadrants**: Conway / Seeds / Maze / Day&Night
3. **Concentric rings**: 10 different rules radiating from center (uses sqrt distance)
4. **Horizontal stripes**: alternating rulesets every 25 rows
5. **Checkerboard**: checkerboard pattern of all 10 rulesets

### Integration Details
- Zone state persists across grid clear (press `c` twice to also clear zones)
- `zone_enabled` flag avoids any performance cost when zones aren't used
- Entering zone mode auto-enables draw mode
- Status bar shows current zone brush with color-coded name
- When zones are active but zone mode is off, shows dim "ZONES" indicator
- Rule display shows "multi-zone" instead of specific rule when zones are active

## Technical Changes
- Added `math.h` for `sqrt()` in concentric ring preset
- Added `-lm` to Makefile
- ~130 lines of new code
- Zero compiler warnings

## Exploration Value
This is genuinely novel for CA explorers — most only support global rule changes. Spatial rule heterogeneity creates boundary physics that don't exist in any single ruleset: Conway gliders encountering Maze corridors, Seeds explosions hitting Day&Night walls, etc. Combined with kaleidoscope symmetry, users can create symmetric rule-domain mandalas.
