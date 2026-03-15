# Round 31: Wolfram Class Detector

**Goal:** Automatic real-time classification of running automaton into Wolfram Classes I–IV by fusing signals from multiple existing analyzers.

**Explore/Exploit:** 20% explore (novel multi-signal fusion algorithm), 80% exploit (leverages rounds 26–30 scientific analysis infrastructure).

## What Was Built

A new overlay (`C` key) that computes a 7-dimensional feature vector from:
- **Shannon entropy** (spatial disorder, sampled at 2× stride)
- **Lyapunov sensitivity** (lightweight 4-probe perturbation test on 80×60 shadow grid)
- **Fractal dimension** (quick 3-scale box-counting: ε=4,16,64)
- **Population variance** (coefficient of variation from ring buffer)
- **Population autocorrelation** (lag-1 ACF for periodicity detection)
- **Population trend** (linear regression slope, normalized)
- **Cell density** (alive/total ratio)

These features feed a weighted scoring system with hand-tuned decision boundaries for each class:

| Class | Profile |
|-------|---------|
| **I: Fixed** | Near-zero density, very low entropy/Lyapunov, declining population |
| **II: Periodic** | Low-moderate entropy, low Lyapunov, high ACF, low population variance |
| **III: Chaotic** | High entropy, high Lyapunov, high D_box, high variance, low ACF |
| **IV: Complex** | Moderate everything — the "edge of chaos" region in feature space |

## UI

- **HUD indicator:** `◆W:III(72%)` showing class and confidence, color-coded by class
- **Panel:** 14-row right-side overlay with:
  - Classification result with confidence percentage
  - Natural language description of the class
  - Full feature vector display (H̄, λ, D_b, σ², ACF, ρ)
  - 4-bar score chart showing relative evidence for each class
- **Color scheme:** Panel border/background changes color based on detected class (blue→green→red→gold)
- Auto-refreshes every 16 generations

## Key Design Decisions

1. **Self-contained computation:** The classifier runs its own lightweight versions of entropy/Lyapunov/fractal analysis rather than depending on those overlays being active. This means it works standalone.
2. **Subsampled for speed:** Entropy uses 2× stride, Lyapunov uses small 80×60 shadow grids with only 4 probes, fractal uses 3 scales instead of 7.
3. **Weighted scoring over hard thresholds:** Each class accumulates evidence from multiple features rather than using a single decision tree, making classification robust to edge cases.
4. **Confidence = winner's share of total score:** Simple but effective measure of classification certainty.

## Files Changed
- `life.c`: +~280 lines (state variables, wolfram_classify() function, panel rendering, key binding, HUD indicator)
- `README.md`: Added `C` key to controls table
