/*
  ==============================================================================

    PhaseControlledOscillator.cpp

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

#include "PhaseControlledOscillator.h"

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
PhaseControlledOscillator<SampleType>::PhaseControlledOscillator()
    : waveShapeFunc (sineWaveFunc<SampleType>)
{
}

//==============================================================================
template <typename SampleType>
void PhaseControlledOscillator<SampleType>::setShape (std::function<SampleType (SampleType)> newWaveShapeFunc)
{
    waveShapeFunc = newWaveShapeFunc;
}

template <typename SampleType>
void PhaseControlledOscillator<SampleType>::setShape (PhaseControlledOscillatorWaveShape newWaveShape)
{
    switch (newWaveShape)
    {
        case PhaseControlledOscillatorWaveShape::sine:
            setShape (sineWaveFunc<SampleType>);
            break;
        case PhaseControlledOscillatorWaveShape::sawtooth:
            setShape (sawtoothWaveFunc<SampleType>);
            break;
    }
}

template <typename SampleType>
void PhaseControlledOscillator<SampleType>::setRate (SampleType newRate)
{
    rate = juce::MathConstants<SampleType>::twoPi * newRate;
}

//==============================================================================
template <typename SampleType>
SampleType PhaseControlledOscillator<SampleType>::getPhase (int delay)
{
    return phase.phase + samplesToPhase (delay);
}

//==============================================================================
template <typename SampleType>
void PhaseControlledOscillator<SampleType>::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = (SampleType) spec.sampleRate;
}

template <typename SampleType>
void PhaseControlledOscillator<SampleType>::reset()
{
    phase.reset();
}

template <typename SampleType>
void PhaseControlledOscillator<SampleType>::advance (SampleType p)
{
    phase.advance (p);
}

//==============================================================================
template <typename SampleType>
SampleType PhaseControlledOscillator<SampleType>::samplesToPhase (int samples)
{
    return rate * samples / sampleRate;
}

//==============================================================================
template class PhaseControlledOscillator<float>;
template class PhaseControlledOscillator<double>;