# Round 46: Vorticity Detection Overlay

**Date**: 2026-03-16
**Explore/Exploit**: 95% explore / 5% exploit

## Goal
Compute the curl of the local density-gradient field to detect and visualize
rotational structures — vortices, spirals, and eddies — hidden in the cellular
automaton dynamics. The project had linear flow analysis (Information Flow Field)
and wave propagation but nothing detecting rotation. Vorticity is the missing
piece connecting the CA to fluid dynamics.

## What was built

### Vorticity Detection (`*` key)
- **Velocity estimation**: Activity-weighted center-of-mass shift in a 5x5
  neighborhood using 2-frame temporal differencing for smoother signal
  - `vx(x,y) = Σ dx · Δactivity / dist` (normalized by total weight)
  - `vy(x,y) = Σ dy · Δactivity / dist`
- **Discrete curl**: ω = ∂vy/∂x − ∂vx/∂y via central differences
- **Vortex detection**: Local maxima of |ω| above adaptive threshold (35% of
  max |ω|, floor 0.02) in 5x5 neighborhoods counted as vortex centers
- **Color scheme**: Blue (clockwise, ω < 0) → dark (irrotational) → red
  (counterclockwise, ω > 0) with adaptive normalization by max |ω|
- **Live cells brightened**: +55 to all channels for visibility
- **Stats panel** (9 rows, purple/indigo theme):
  - max |ω| and mean vorticity
  - Net circulation (Γ = Σω) and vortex count
  - Color gradient legend with CW/CCW rotation labels
  - Max vorticity sparkline history (64-frame buffer)
  - Toggle hint
- **Auto-refresh**: Every 2 generations (needs temporal differencing)
- **Cell probe inspector**: Shows ω value with CW/CCW/irrotational label

### What it reveals
- **Spiral gliders** produce distinct rotational signatures invisible to
  linear flow or wave analysis
- **Oscillator halos** show alternating CW/CCW vorticity rings
- **Chaotic boundaries** between active regions generate turbulent vorticity
- **Net circulation** (Γ) measures global rotational asymmetry — systems with
  more CW than CCW rotation show negative Γ, and vice versa

## Technical details
- Zero heap allocation (static arrays for vorticity, velocity, and history)
- Two-frame temporal history for velocity estimation (prev and prev2 snapshots)
- Boundary conditions: zero vorticity at grid edges
- Adaptive vortex threshold prevents noise from being counted as structure
- Panel stacking updated for wave mechanics panel to account for vorticity height

## Lines
- Before: 12,035
- After: 12,433
- Added: +398 lines

## Classification
~95% exploration — introduces discrete vector calculus (curl operator) over the
CA dynamics, connecting to fluid dynamics and turbulence theory. The project had
no rotational analysis before; this completes the vector field toolkit alongside
the existing divergence-like (flow) and scalar (wave) overlays.
