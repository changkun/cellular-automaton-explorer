# Round 23: Spaceship & Glider Detection

## Goal
Extend the pattern census overlay (round 19) to detect *moving* structures — gliders and spaceships — by comparing consecutive timeline frames. The census previously recognized only static still lifes and oscillators; spaceships are the most visually dramatic objects, especially on exotic topologies (round 22) where gliders wrap through Klein bottles and Möbius strips with direction reversals.

## What Was Built

### Spaceship Templates
4 spaceship types defined with full phase patterns:
- **Glider** — 3×3, 4 phases, displacement (+1,+1) per cycle
- **LWSS** (lightweight spaceship) — 5×4, 4 phases, displacement (+2,0) per cycle
- **MWSS** (middleweight spaceship) — 6×5, 4 phases, displacement (+2,0) per cycle
- **HWSS** (heavyweight spaceship) — 7×5, 4 phases, displacement (+2,0) per cycle

### Two-Frame Motion Detection (`census_ship_scan()`)
- Compares timeline frame N−1 and frame N
- Matches a phase template in the previous frame, confirms the next phase appears at a nearby offset (±2 cells) in the current frame
- Topology-aware dead-border checks via `topo_map()`
- Skips cells already claimed by static census patterns
- Runs automatically after the static census scan completes

### Visual Presentation
- **Cyan cell coloring**: Detected spaceship cells render in bright cyan (RGB 0/176/200 – 0/204/255), modulated by cell age for depth — distinct from green (still lifes) and amber (oscillators)
- **Direction arrows**: Unicode arrows (↑↗→↘↓↙←↖) rendered at each spaceship's bounding-box center, showing travel direction
- **Census panel**: Entries color-coded by type field (0=still life/green, 1=oscillator/amber, 2=spaceship/cyan); spaceship counts included in totals

### Data Structures
- `ShipType` / `ShipPhase`: Template definitions with per-phase bit patterns and per-cycle displacement vectors
- `ShipDetection`: Runtime detection records storing position, bounding box, direction index, and type
- `census_ship[MAX_H][MAX_W]`: Overlay grid marking which cells belong to detected spaceships
- `census_display` expanded from 10 → 16 entries with added `type` field

## Topology Interaction
On a Klein bottle, a glider exiting the top re-enters mirrored from the bottom — the tracker detects both the original and reflected glider as separate instances with opposite direction arrows, making the topology's effect on motion visible.

## Files Modified
- `life.c`: Added spaceship templates, `census_ship_scan()`, `ship_match_at()`, `ship_dir_from_delta()`, cyan coloring in `cell_color()`, arrow overlay and type-colored panel entries in `render()`
