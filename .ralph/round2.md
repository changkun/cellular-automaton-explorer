---
round: 2
goal: "Mouse-driven cell painting, population sparkline graph, and toroidal wrapping"
status: complete
---

## What was built
- **Mouse support**: Left-click/drag to place cells, right-click/drag to erase — uses SGR extended mouse protocol (1006) for large terminal support
- **Population sparkline**: Live scrolling graph in the status bar using Unicode block elements (▁▂▃▄▅▆▇█) with color gradient (red→yellow→green)
- **Toroidal wrapping**: Toggle with `w` — cells wrap around edges, shown with ∞ indicator
- **Draw mode toggle**: `d` to enable/disable mouse interaction
- **Graph toggle**: `g` to show/hide sparkline
- Refactored input handling to cleanly separate keyboard and mouse events
- Added second status line with compact key reference

## Future directions
- Stamp mode: pick a pattern (glider, etc.) and stamp it at mouse position
- Cell age coloring (heatmap based on how long a cell has been alive)
- Save/load state to file
- Rule variants (HighLife, Day & Night, etc.)
- Undo/redo system
- Network multiplayer (shared grid via sockets)
