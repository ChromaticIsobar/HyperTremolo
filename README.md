# HyperTremolo
HyperTremolo is a Dual Amplitude Modulator effect. It is designed to be an Harmonic Tremolo, but can be tweaked to get a lot of different effects, from haunting Ring Modulations to Rotary Speaker emulations!

![Screenshot](https://gist.githubusercontent.com/ChromaticIsobar/6ce8126b4bc9a02594282bde2909f02b/raw/HyperTremolo.png)

<!--
## Download instructions
TODO
-->

## The Effect
HyperTremolo splits your input signal into a low-passed and a high-passed signal. Each of these
signals is modulated with an amplitude modulator (tremolo). Then the two signals are mixed back together.

![Diagram](https://gist.githubusercontent.com/ChromaticIsobar/6ce8126b4bc9a02594282bde2909f02b/raw/HyperTremoloDiagram.png)

<!--
## Installation guide
TODO
-->

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
 - __Balance__: the mix of the low-pass and high-pass signals. With a balance of 1, only the low-pass signal is mixed. With a balance of 0, only the hi-pass signal is mixed
 - __Harmonic__:  The mix between the filtered signal and the bypassed signal. With values close to 1, the effect is more similar to an _harmonic tremolo_. With values close to 0, the effect is more similar to an ordinary _tremolo_

### Mixer
 - __Gain__: Gain for the wet signal
 - __Mix__: The mix between the wet and dry signals

<!--
## Known Limitations
TODO
-->

<!--
## Example Settings
TODO
-->

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
