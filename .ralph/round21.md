# Round 21: Braille Ultra-Density Rendering

## Goal
Add a Braille rendering mode that uses Unicode Braille characters (U+2800–U+28FF) to pack 2×4 cells per terminal character, achieving 8x pixel density — the highest zoom-out level in the application.

## What was built
- **Braille zoom level (8x)**: New rendering path using Unicode Braille glyphs where each character encodes a 2-wide × 4-tall block of cells as an 8-dot pattern
- **Viewport scaling**: On a 200×50 terminal, braille zoom shows the entire 400×200 grid in a single view
- **Color blending**: Each braille glyph is colored with the average RGB of all active cells in its 2×4 block, using 24-bit terminal color
- **Zoom cycle extended**: `z`/`x` keys and mouse scroll now cycle through 1 → 2 → 4 → 8
- **Pan step scaling**: 32-cell pan steps at braille zoom for proportional navigation
- **Mouse-to-grid conversion**: Correct 2x horizontal, 4x vertical scaling for draw mode at braille zoom

## Key design decisions
- **Braille dot encoding follows Unicode standard**: Column-major bit layout (dot1=0x01 at (0,0), dot2=0x02 at (0,1), ..., dot8=0x80 at (1,3)) maps directly to cell positions in the 2×4 block
- **Average color rather than dominant**: When multiple colored cells exist in a block, their RGB values are averaged — this preserves heatmap gradients and species color mixing at high zoom
- **Ghost trails and zone backgrounds rendered as dots**: All cell types (alive, ghost, zone, emitter, absorber, tracer) contribute dots and color, maintaining visual consistency with other zoom levels
- **UTF-8 encoding computed inline**: `0xE2, 0xA0+(dots>>6), 0x80+(dots&0x3F)` avoids any lookup table or library dependency

## Integration points
- Uses existing `cell_color()` dispatch — all coloring modes (heatmap, species, frequency, zone) compose naturally
- Status bar shows `⌂8x` at braille zoom (automatic from existing zoom display logic)
- Follows the same render buffer pattern as half-block and quarter-block modes
- Mouse interaction and stamp placement work correctly at 8x zoom

## Technical notes
- ~55 lines of new render code in the `zoom == 8` branch
- 8 cells sampled per terminal character with a coordinate lookup table for the Braille bit mapping
- Empty blocks render as spaces (no escape sequences), keeping output compact for sparse regions
- Braille characters are 3 bytes in UTF-8, so buffer sizing remains well within existing limits
