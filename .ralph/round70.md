# Round 70 — Topological Defect Tracker

## Goal
Add a Topological Defect Detection overlay that identifies, classifies, and
tracks point defects (vortices), domain walls, and saddle points in the
automaton's orientation/order parameter field using discrete winding numbers.

## What was built
- **Overlay #37** (ghost layer) — Toggle key `Ctrl-D`
- Orientation field computation via Sobel-weighted density gradients
- Discrete winding number calculation on 2×2 plaquettes
- Domain wall detection from angular disagreement between neighbors
- Defect classification: +1 vortex, -1 vortex, domain wall, ±2 saddle
- Net topological charge tracking with conservation violation counter
- Pair creation/annihilation rate estimation with exponential smoothing
- Full sidebar panel with census, charge, rates, legend, dual sparklines
- Ghost layer rendering (code 37) with pink/cyan/yellow/white color scheme

## Algorithm
1. Sobel gradient → orientation angle θ(x,y) = atan2(∂ρ/∂y, ∂ρ/∂x)
2. For each 2×2 plaquette: sum wrapped angle differences around circuit
3. Total / 2π rounded → winding number (integer topological charge)
4. Domain walls: max neighbor angular difference > π/2
5. Track net charge Q, creation/annihilation from total defect count changes

## Technical details
- ~480 lines added (21,353 → ~21,830)
- Zero dynamic allocation, all static arrays
- Lazy recomputation via td_stale flag
- Integrated into split-screen system as overlay index 36
- Panel stacks below all existing panels

## Explore/Exploit: 95% explore / 5% exploit
- Entirely new analytical domain: condensed matter defect physics
- Winding number computation is new mathematical framework
- Exploits existing grid infrastructure and overlay pattern

## Connections
- Complements Symmetry Group (R69): symmetry shows where order exists,
  defects show where it breaks down
- Complements Fisher Information (R68): Fisher shows sensitivity at
  boundaries, defects show the topological structure of those boundaries
- Complements Vorticity (R16): vorticity measures rotation strength,
  defect tracker measures topological charge (quantized, conserved)
