# Round 18: Screenshot & Animation Capture (PPM Export)

## Goal
Add a frame capture system so users can save and share the visual output of all 17 rounds of rendering features. Press `P` to screenshot the current viewport as a PPM image, or `Ctrl-P` to dump the entire timeline buffer as a numbered image sequence.

## What was built

### Single Frame Capture (`P` key)
- **`capture_frame()`** renders the current viewport through `cell_color()` and writes a binary PPM (`P6`) file
- Auto-numbered as `frame_0001.ppm`, `frame_0002.ppm`, etc., skipping existing files
- Status bar flash: `"Saved frame_0042.ppm (320x200)"`

### Timeline Sequence Export (`Ctrl-P`)
- **`capture_timeline()`** iterates the entire 256-frame timeline ring buffer (oldest to newest)
- Temporarily restores each historical frame, renders it, then restores the original grid state
- Produces a numbered image sequence: `frame_0001.ppm` through `frame_0256.ppm`
- Status bar flash: `"Saved 256 frames (320x200) frame_0001..0256.ppm"`

### PPM Format
- Binary PPM (`P6`): header line + raw RGB bytes, zero dependencies
- Readable by ImageMagick, GIMP, ffmpeg, feh, and most image tools
- Convert to animated GIF: `convert -delay 5 frame_*.ppm animation.gif`
- Convert to video: `ffmpeg -framerate 30 -i frame_%04d.ppm output.mp4`

## Technical details
- Forward-declares `cell_color()` to access the unified rendering pipeline
- Image dimensions match the current viewport (1 cell = 1 pixel)
- `next_frame_number()` scans for the first unused slot (up to 9999)
- Timeline capture saves/restores full grid state (grid, ghost, species, generation, population) around the export loop
- `P` key separated from `p` (play/pause) — case-sensitive distinction
- `Ctrl-P` mapped to key code 16

## Interactions with existing features
- **All rendering modes**: Captures whatever `cell_color()` produces — heatmap, species coloring, zone tinting, frequency overlay, signal tracer, portal animations, ghost trails
- **Timeline replay (round 10)**: `Ctrl-P` exports the full history buffer as an image sequence
- **Viewport/zoom (round 6)**: Captures exactly what the viewport shows at current pan/zoom position
- **Ecosystem mode (round 16)**: Species colors (Red vs Blue) preserved in captures
- **Pattern stamps (round 17)**: Screenshot stamp compositions for sharing
