# Round 29: 2D Spatial Fourier Spectrum Analyzer

## Goal
Shift from real-space analysis (entropy, temperature, Lyapunov) to **frequency space** — a fundamentally different lens on the same automaton. A real-time 2D discrete Fourier transform reveals dominant spatial frequencies, periodic wavelengths, and structural regularity. Gliders have a characteristic wavelength, oscillators appear as spectral peaks, random soup is white noise, and the phase transition at critical temperature (~0.15) shows a dramatic shift from peaked to broadband spectrum.

## What Was Built

### Core Algorithm — Cooley-Tukey Radix-2 FFT
- `fft1d()` — in-place 1D FFT with bit-reversal permutation and butterfly stages
- Applied to rows then columns for a full 2D transform on a 512×256 zero-padded grid
- ~1.5MB static workspace (`fft_re`/`fft_im` at `FFT_W×FFT_H` floats each)
- Auto-refreshes every 8 generations when active

### Power Spectrum Overlay
- Log-scaled 2D power spectrum (fftshifted, DC at center) mapped onto the grid
- `fourier_to_rgb()` color mapping: dark indigo (low) → blue → cyan → green → yellow → white (high)
- Alive cells brightened (+35 per channel) to remain visible over spectrum wash
- Color return code 12 for "fourier ghost" cells (dead but spectrally active)

### Overlay Panel (18 rows, top-right)
- **Spectral entropy** (H_s) and **dominant wavelength** (λ) in cells
- **Radial power profile** — 4-row histogram (40 bins) showing azimuthally-averaged power vs spatial frequency (DC → Nyquist), colored by frequency band
- **2D spectrum mini-view** — 8×40 char compact spectral density plot using Unicode half-block rendering
- **Spectral entropy bar** — 30-char visual fill gauge with gradient coloring

### Spectral Metrics
- **Spectral entropy** — normalized to [0,1]: 0 = peaked/crystalline, 1 = broadband/noise. Computed from radial power distribution (excluding DC bin) using Shannon entropy formula
- **Dominant wavelength** — peak spatial period in cells, found from maximum of radial profile (excluding DC). Converted from frequency bin via `λ = N_avg / k_peak`
- **Radial power profile** — 128 bins of azimuthally-averaged power, binned by Euclidean distance from DC in frequency space

### Controls
- `u` — Toggle Fourier spectrum analyzer (`F` was already taken by frequency counter)

### HUD
- `≈FFT:λNN.N` shown in status bar when active (cyan color)

### Panel Stacking
- Stacks below entropy, temperature, and Lyapunov panels when multiple overlays are active

## Connections to Previous Rounds
- **Shannon entropy** (round 26): spectral entropy is the frequency-domain complement — one measures information content in space, the other measures energy distribution across frequencies
- **Temperature field** (round 27): the phase transition at critical T should show a dramatic spectral shift from peaked (ordered) to broadband (chaotic)
- **Lyapunov sensitivity** (round 28): Lyapunov measures temporal divergence; Fourier reveals spatial periodicity — orthogonal views of the same dynamics

## Files Modified
- `life.c`: ~370 lines added (FFT workspace and globals, fft1d Cooley-Tukey implementation, fourier_compute with 2D FFT + radial binning + spectral entropy, fourier_to_rgb color mapping, cell_color Fourier overlay, render panel with radial histogram + mini spectrum + entropy bar, HUD indicator, key binding for `u`, auto-refresh logic, help bar update)
