---
round: 1
goal: "Terminal Conway's Game of Life in C with interactive controls"
status: complete
---

## What was built
- `life.c` — fully interactive terminal Game of Life
- `Makefile` — simple build system
- Compiles with gcc, zero external dependencies
- Features: play/pause, step, randomize, clear, 5 preset patterns (glider, pulsar, gosper gun, r-pentomino, acorn), speed control, terminal resize handling, color-cycling cells, ~60fps rendering

## Future directions
- Mouse click to toggle cells
- Toroidal wrapping mode
- Save/load state to file
- Rule variants (HighLife, Day & Night, etc.)
- Population graph / stats overlay
- Network multiplayer (shared grid via sockets)
