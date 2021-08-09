/*
  ==============================================================================

    Tremolo.cpp
    
    Copyright (c) 2021 Marco Tiraboschi
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.

  ==============================================================================
*/

#include "Tremolo.h"

//==============================================================================
template <typename SampleType>
SampleType sineWaveFunc (SampleType phase)
{
    return (std::sin (phase) + ((SampleType) 1)) / ((SampleType) 2);
}

template <typename SampleType>
SampleType sawtoothWaveFunc (SampleType phase)
{
    return fmod (
        phase / juce::MathConstants<SampleType>::twoPi,
        (SampleType) 1.0);
}

//==============================================================================
template <typename SampleType>
Tremolo<SampleType>::Tremolo()
    : waveShapeFunc (sineWaveFunc<SampleType>)
{
}

//==============================================================================
template <typename SampleType>
void Tremolo<SampleType>::setRate (SampleType newRateHz)
{
    rate = juce::jmax (newRateHz, (SampleType) 0);
}

template <typename SampleType>
void Tremolo<SampleType>::setShape (
    std::function<SampleType (SampleType)> newWaveShapeFunc)
{
    waveShapeFunc = newWaveShapeFunc;
}

template <typename SampleType>
void Tremolo<SampleType>::setShape (TremoloWaveShape waveShape)
{
    if (waveShape == TremoloWaveShape::sawtooth)
        return setShape (sawtoothWaveFunc<SampleType>);
    setShape (sineWaveFunc<SampleType>);
}

template <typename SampleType>
void Tremolo<SampleType>::setMix (SampleType newMix)
{
    dryWet.setWetMixProportion (newMix);
}

//==============================================================================
template <typename SampleType>
void Tremolo<SampleType>::prepare (const juce::dsp::ProcessSpec& spec)
{
    dryWet.prepare (spec);
    sampleRate = spec.sampleRate;
}

template <typename SampleType>
void Tremolo<SampleType>::reset()
{
    dryWet.reset();
    phase.reset();
}

template <typename SampleType>
void Tremolo<SampleType>::advance (SampleType p)
{
    phase.advance (juce::jmax (p, (SampleType) 0));
}

//==============================================================================
template <typename SampleType>
SampleType Tremolo<SampleType>::phaseAfter (size_t delaySamples)
{
    return fmod (
        phase.phase + samplesToPhase (delaySamples),
        juce::MathConstants<SampleType>::twoPi);
}

template <typename SampleType>
SampleType Tremolo<SampleType>::samplesToPhase (size_t s)
{
    return juce::MathConstants<SampleType>::twoPi * rate * s / sampleRate;
}

//==============================================================================
template class Tremolo<float>;
template class Tremolo<double>;
