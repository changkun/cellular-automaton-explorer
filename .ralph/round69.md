# Round 69: Symmetry Group Detection — Discrete Symmetry Classification

## Goal
Add a Symmetry Group Detection overlay that identifies and visualizes local discrete symmetry classes (C1, C2, C4, D1, D2, D4, D8) across the grid in real-time, opening a new analytical domain: **group theory and spontaneous symmetry breaking**.

## What was built
A new analytical overlay (toggle `Ctrl-G`, ghost layer 36, split overlay index 35) that classifies every cell's 5×5 neighborhood into its discrete symmetry group and tracks symmetry-breaking events.

### Core algorithm: `sg_compute()` (~110 lines)

**Symmetry testing:**
- Extracts 5×5 patch centered at each cell (out-of-bounds treated as dead)
- Tests all 6 symmetry transformations simultaneously per cell:
  - **Rot90**: patch[r][c] == patch[c][4-r]
  - **Rot180**: patch[r][c] == patch[4-r][4-c]
  - **FlipH**: patch[r][c] == patch[4-r][c] (horizontal axis)
  - **FlipV**: patch[r][c] == patch[r][4-c] (vertical axis)
  - **FlipD**: patch[r][c] == patch[c][r] (main diagonal)
  - **FlipA**: patch[r][c] == patch[4-c][4-r] (anti-diagonal)
- Early termination: inner loop breaks as soon as all failing tests are identified

**Group classification (9 groups):**
- D4 (order 8): rot90 + all 4 reflections — full dihedral symmetry of the square
- C4 (order 4): rot90 without reflections — pure 4-fold rotation
- D2 (order 4): rot180 + at least 2 reflections
- C2 (order 2): 180° rotation only
- D1h/D1v/D1d/D1a (order 2): single reflection axis (horizontal/vertical/diagonal/anti-diagonal)
- C1 (order 1): identity only — no symmetry

**Symmetry-breaking detection:**
- Stores previous frame's per-cell symmetry order
- Counts cells transitioning from higher to lower order (breaking) and vice versa (forming)
- Exponentially smooths breaking rate (α=0.15) for stable display

### Rendering
**Cell coloring by symmetry order:**
- C1 (ord 1): dark charcoal with blue tint — asymmetric
- C2/D1 (ord 2): cool blue-cyan spectrum, varies by specific group subtype
- D2/C4 (ord 4): warm green (D2) or gold (C4, pure rotation is special)
- D4 (ord 8): brilliant white-gold glow
- Live cells receive +55 brightness boost

### Sidebar panel (11 rows, gold-green theme)
- Row 1: Mean symmetry order Ō + classification (ASYMMETRIC/LOW/MODERATE/HIGH)
- Row 2: Dominant non-C1 group + breaking/forming rates (↓/↑ percentages)
- Row 3: Separator
- Rows 4-6: 9-group distribution histogram (3×3 grid, colored bars with percentages)
- Row 7: Color legend gradient (charcoal → blue → green → gold → white)
- Row 8: Scale labels (C1, C2/D1, D2/C4, D4)
- Row 9: Sparkline history of Ō(t) normalized to [1,8]
- Bottom border

### Integration
- `sg_stale` flag set every generation advance
- Recomputed every 4 generations when active (5×5 per-cell is O(25·H·W))
- Split-screen: index 35, fully supported in split_set_overlay/detect/ensure
- Key: Ctrl-G (key code 7) — "G" for Group theory

## Analytical significance
This overlay opens a completely new domain not touched by any of the 35 prior overlays. Spontaneous symmetry breaking is fundamental to:
- **Condensed matter**: crystal formation, magnetic domains, liquid crystals
- **Particle physics**: the Higgs mechanism, electroweak symmetry breaking
- **CA dynamics**: where ordered structures form vs. where chaos disrupts regularity

Complements Fisher Information (which measures sensitivity at phase boundaries) by revealing *what kind of structural order* exists on each side of those boundaries. Together they answer: "Where are the phase boundaries?" (Fisher) and "What symmetries characterize each phase?" (SymmGroup).

## Stats
- Lines added: ~480
- New overlay: #36 (SymmGroup)
- Key binding: Ctrl-G
- Exploration factor: ~95% (entirely new analytical domain — group theory)
