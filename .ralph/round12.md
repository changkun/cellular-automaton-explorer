# Round 12: Interactive Rule Editor Overlay

**Explore/exploit**: 12% explore — pure exploitation, deepening the rule engine (round 4) and zone system (round 8).

## Goal
Add a visual, clickable birth/survival rule editor overlay that replaces random mutation with precise control.

## What was built
- **Rule editor overlay** (`b` key toggle): A compact, styled overlay panel rendered over the live simulation
- **Clickable B/S bit grid**: Two rows of digits (0-8) for birth and survival conditions. Click any digit to toggle that bit in the corresponding mask. Active bits highlighted with colored backgrounds (green=birth, blue=survival).
- **Clickable preset names**: 10 preset rulesets displayed in two rows of 5. Click any name to load that ruleset instantly. Current active preset highlighted.
- **Live feedback**: Simulation continues running underneath the overlay — changes take effect immediately, so you see the rule's impact in real time.
- **Overlay rendering**: Box-drawing characters (┌─┐│└─┘) with dark semi-transparent background. Absolute cursor positioning ensures clean overlay on top of any zoom level.

## Key implementation details
- State: `rule_editor` flag, `re_col0`/`re_row0` for overlay geometry
- Rendering: `render_rule_editor()` appends ANSI escape sequences to the render buffer, using absolute cursor positioning for each row
- Click detection: `rule_editor_click()` maps terminal coordinates to B/S digit indices; `rule_editor_preset_click()` maps to preset names by column position
- Mouse interception: Rule editor clicks are handled before grid drawing, using a `goto mouse_done` to skip normal mouse handling
- Right border alignment: `RE_CLOSE_ROW` macro pads content and positions the right │ absolutely

## Lines changed
- ~150 lines added to life.c (now ~2,210 lines total)
- New key `b` added to control comments and help bar
- README updated with rule editor documentation

## Why this matters
Transforms rule exploration from slot-machine randomness (pressing `m` repeatedly) into precise craftsmanship. Combined with zone-paint mode, users can now design exact rules for each spatial region, enabling intentional multi-rule ecosystems instead of accidental ones.
