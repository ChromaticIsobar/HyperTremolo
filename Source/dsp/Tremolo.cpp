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
Tremolo<SampleType>::Tremolo()
{
    setThroughZero (false);
}

//==============================================================================
template <typename SampleType>
void Tremolo<SampleType>::setRate (SampleType newRateHz)
{
    lfo.setRate (newRateHz);
}

template <typename SampleType>
void Tremolo<SampleType>::setThroughZero (bool newThroughZero)
{
    if (newThroughZero)
    {
        amScale.setGainLinear (2);
        amBias.setBias (-1);
    }
    else
    {
        amScale.setGainLinear (1);
        amBias.setBias (0);
    }
}

template <typename SampleType>
void Tremolo<SampleType>::setShape (
    std::function<SampleType (SampleType)> waveShapeFunc)
{
    lfo.setShape (waveShapeFunc);
}

template <typename SampleType>
void Tremolo<SampleType>::setShape (PhaseControlledOscillatorWaveShape waveShape)
{
    lfo.setShape (waveShape);
}

template <typename SampleType>
void Tremolo<SampleType>::setMix (SampleType newMix)
{
    dryWet.setWetMixProportion (newMix);
}

template <typename SampleType>
void Tremolo<SampleType>::setOffset (SampleType v)
{
    lfo.setOffset (v);
}

template <typename SampleType>
void Tremolo<SampleType>::setOffset (SampleType tv, SampleType cv)
{
    lfo.setOffset (tv, cv);
}

template <typename SampleType>
SampleType Tremolo<SampleType>::getOffset()
{
    return lfo.getOffset();
}

//==============================================================================
template <typename SampleType>
SampleType Tremolo<SampleType>::getPhase()
{
    return lfo.getPhase();
}

//==============================================================================
template <typename SampleType>
void Tremolo<SampleType>::prepare (const juce::dsp::ProcessSpec& spec)
{
    dryWet.prepare (spec);
    lfo.prepare (spec);
    amBuffer.reset (
        new juce::AudioBuffer<SampleType> (
            spec.numChannels, spec.maximumBlockSize));

    amScale.prepare (spec);
    amBias.prepare (spec);
    amScale.setRampDurationSeconds (rampLength);
    amBias.setRampDurationSeconds (rampLength);
}

template <typename SampleType>
void Tremolo<SampleType>::reset()
{
    lfo.reset();
    dryWet.reset();
    amScale.reset();
    // amBias.reset();
}

template <typename SampleType>
void Tremolo<SampleType>::advance (SampleType p)
{
    lfo.advance (p);
}

//==============================================================================
template class Tremolo<float>;
template class Tremolo<double>;
