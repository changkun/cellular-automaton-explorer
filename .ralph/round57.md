# Round 57 — 3D Strange Attractor Viewer

## Goal
Extend the 2D phase portrait (Round 53) into a rotating 3D point cloud of metric trajectories, revealing attractor geometry invisible in any single 2D projection.

## What Was Built
- **3D Strange Attractor overlay** (toggle: `/` key)
  - Pick any 3 of the 16 scalar metrics as X/Y/Z axes
  - 256-point trajectory history in a ring buffer
  - Real-time 3D projection with perspective (weak perspective, camera at z=4)
  - Camera rotation via yaw/pitch angles
  - Auto-rotate mode (slow continuous yaw rotation while simulation runs)
  - Depth-based coloring: near points bright cyan, far points dim
  - Recency fading: older points dimmer than newer ones
  - Braille dot rendering: 36×16 char canvas = 72×64 dot resolution

## Controls
- `/` — toggle 3D attractor on/off
- `<`/`>` — cycle X/Y metric (overrides phase portrait when sa_mode active)
- `{`/`}` — cycle Z metric (overrides temperature/interaction when sa_mode active)
- Arrow keys — manual pitch/yaw rotation (disables auto-rotate)
- `R` — toggle auto-rotate on/off (overrides randomize when sa_mode active)

## Key Design Decisions
1. **Reuses `pp_read_metric()` interface** — zero new metric code, just adds a Z dimension
2. **Weak perspective projection** — camera at z=4 gives natural-looking depth without extreme distortion
3. **Context-sensitive key overloading** — `<`/`>`/`{`/`}`/`R`/arrows behave differently when sa_mode is active, preserving the tight keybinding space
4. **Auto-rotate** — slowly rotating the attractor reveals 3D structure automatically; manual control available via arrows
5. **Larger canvas than 2D phase portrait** — 36×16 Braille chars (vs 30×12) for more detail in 3D

## Integration Points
- Globals: after recurrence plot section (~line 909)
- Record function: after pp_record (~line 1375)
- Rendering: after recurrence plot panel (~line 14148)
- Key dispatch: multiple locations (sa_mode checks added before existing handlers)
- Status bar: sa_str indicator added to status line
- Demo reset: sa_mode = 0 added

## Lines Added
~350 lines

## Exploration vs Exploitation
- **90% explore / 10% exploit**: Entirely new rendering paradigm (3D projection + rotation) that's never been done in this project. Reuses existing metric infrastructure (exploit) but the 3D math, perspective projection, depth coloring, and auto-rotation are completely new territory.

## Key Insight
The 2D phase portrait collapses 3D attractor structure into a shadow — what appears as a tangled mess in 2D may reveal itself as a clean toroidal orbit or strange attractor when viewed in 3D with rotation. The auto-rotate feature is crucial: static 3D projections are hard to interpret, but continuous rotation gives the brain enough parallax cues to perceive depth.
