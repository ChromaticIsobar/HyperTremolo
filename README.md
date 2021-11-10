# HyperTremolo

[![Projucer CI/CD Badge](https://github.com/ChromaticIsobar/HyperTremolo/actions/workflows/main.yml/badge.svg)](https://github.com/ChromaticIsobar/HyperTremolo/actions/workflows/main.yml)<!-- DELETE -->
[![Download Badge](https://img.shields.io/badge/download-zip-blueviolet)](https://chromaticisobar.github.io/HyperTremolo/)<!-- DELETE -->

HyperTremolo is a Dual Amplitude Modulator effect. It is designed to be an Harmonic Tremolo, but can be tweaked to get a lot of different effects, from haunting Ring Modulations to Rotary Speaker emulations!

![Screenshot](https://gist.githubusercontent.com/ChromaticIsobar/6ce8126b4bc9a02594282bde2909f02b/raw/HyperTremolo.png)

## The Effect
HyperTremolo splits your input signal into a low-passed and a high-passed signal. Each of these signals is modulated with an amplitude modulator (tremolo). Then the two signals are mixed back together.

![Diagram](https://gist.githubusercontent.com/ChromaticIsobar/6ce8126b4bc9a02594282bde2909f02b/raw/HyperTremoloDiagram.png)

<div style="page-break-after: always;"></div>

## Parameters
The parameters are divided in three sections

### Tremolo
 - __Through-0__: when this toggle is on, the tremolo will go through zero. In other words, the amplitude modulation will be between -1 and +1. When it is off, the modulation is between 0 and +1
 - __Rate__: this is the amplitude modulation frequency (of the low-pass signal)
 - __Ratio__: this is the ratio between the amplitude modulation frequencies of the low-pass signal and of the high-pass signal. When ratio is greater than 1, the high-pass signal will be modulated faster then the low-passed signal (useful to get a Rotary-speaker emulation effect)
   - __Sync__: in the center of the _ratio_ knob there is a _sync_ button. This syncronizes the phases of the two tremolos to be at a difference of π/2. Click on this when setting the _ratio_ back to 1 to align the tremolos in such a way to get a _harmonic tremolo_ effect  
 - __Depth__:  The mix between the modulated signal and the dry signal

### Filter
 - __Crossover__: the cutoff frequency of the low-pass and high-pass filters
 - __Resonance__: the resonance of the low-pass and high-pass filters
 - __Balance__: the mix of the low-pass and high-pass signals. With a balance of 0, only the low-pass signal is mixed. With a balance of 1, only the hi-pass signal is mixed
 - __Harmonic__:  The mix between the filtered signal and the bypassed signal. With values close to 1, the effect is more similar to an _harmonic tremolo_. With values close to 0, the effect is more similar to an ordinary _tremolo_

### Mixer
 - __Gain__: Gain for the wet signal
 - __Mix__: The mix between the wet and dry signals

<div style="page-break-after: always;"></div>

<!--## Known Limitations -->

## Example Settings
|                    | Through-0 | Rate | Ratio | Sync | Depth | Crossover | Resonance | Balance | Harmonic | Gain | Mix  |
|--------------------|-----------|------|-------|------|-------|-----------|-----------|---------|----------|------|------|
| Fast Pulsar        |     ✗     | 6.00 | 1.00  |   ✓  | 1.00  |  1000.0   |   0.707   |  0.50   |   1.00   | 3.00 | 1.00 |
| Slow Phaser        |     ✗     | 0.75 | 1.00  |   ✓  | 0.90  |  1000.0   |   0.707   |  0.50   |   1.00   | 3.00 | 1.00 |
| Subtle Rotary      |     ✗     | 1.33 | 4.50  |   ✗  | 0.75  |  1000.0   |   0.707   |  0.60   |   1.00   | 3.00 | 1.00 |
| Rotary Cabinet     |     ✓     | 1.33 | 4.50  |   ✗  | 1.00  |  1000.0   |   1.25    |  0.50   |   1.00   | 0.00 | 1.00 |
| Dual Trem          |     ✗     | 2.00 | 4.00  |   ✓  | 1.00  |  1000.0   |   0.707   |  0.50   |   0.00   | 0.00 | 1.00 |
| Glitchy Resonances |           | 6.28 | 10.00 |   ✗  | 1.00  |   200.0   |   0.707   |  0.333  |   0.50   | 3.00 | 1.00 |

## Setup instructions
You can find the latest release on GitHub at https://github.com/ChromaticIsobar/HyperTremolo/releases

### Download
You can either download only the specific plugin (standalone, VST3, or  Audio Unit) for your OS or the whole release in a single zip file: `HyperTremolo_v<x>.<y>.<z>.zip`. This file contains:
- `HyperTremolo_UserManual_v<x>.<y>.<z>.pdf`: the user manual PDF file for the specific version
- `Changelog.txt`: a summary of changes between different plugin versions
- `Windows`: a folder of precompiled binaries for Windows
  - `Win32`: a folder of precompiled binaries for 32 bit Windows
      - `HyperTremolo.exe`: the standalone executable for 32 bit Windows
      - `HyperTremolo.vst3`: the VST3 plugin for 32 bit Windows
  - `x64`: a folder of precompiled binaries for 64 bit Windows
      - `HyperTremolo.exe`: the standalone executable for 64 bit Windows
      - `HyperTremolo.vst3`: the VST3 plugin for 64 bit Windows
- `MacOS`: a folder of precompiled binaries for MacOS
  - `HyperTremolo.app`: the standalone executable for MacOS
  - `HyperTremolo.vst3`: the VST3 plugin for MacOS
  - `HyperTremolo.component`: the Audio Unit plugin for MacOS
- `Linux`: a folder of precompiled binaries for Ubuntu
  - `HyperTremolo`: the standalone executable for Ubuntu
  - `HyperTremolo.vst3`: the VST3 plugin for Ubuntu

### Install
Standalone applications require no installation. Plugins must be copied to the proper directory to be installed

|    OS   |    Plugin    |                   Path                   |
|---------|--------------|------------------------------------------|
| Windows | VST3 (x64)   | C:\Program Files\Common Files\VST3       |
| Windows | VST3 (Win32) | C:\Program Files (x86)\Common Files\VST3 |
| Ubuntu  | VST3         | ~/.vst                                   |
| MacOS   | VST3         | Library/Audio/Plug-ins/VST3              |
| MacOS   | AU           | Library/Audio/Plug-ins/Components        |

### Build from source<!-- DELETE -->
Check out [`BUILD.md`](BUILD.md) for instructions on how to build from source<!-- DELETE -->

<div style="page-break-after: always;"></div>

## Contacts
This software was developed by Marco Tiraboschi at the _Laboratorio di Informatica Musicale_
 - mail: marco.tiraboschi@unimi.it
 - website: https://www.lim.di.unimi.it

For issues about this specific software, please open an issue on GitHub

https://github.com/ChromaticIsobar/HyperTremolo

## License
Copyright (c) 2021 Marco Tiraboschi

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
