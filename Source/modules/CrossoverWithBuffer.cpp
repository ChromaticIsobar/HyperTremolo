/*
  ==============================================================================

    CrossoverWithBuffer.cpp
    
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

#include "CrossoverWithBuffer.h"

//==============================================================================
template <typename SampleType>
CrossoverWithBuffer<SampleType>::CrossoverWithBuffer()
    : process_lpf ([] (juce::dsp::ProcessContextReplacing<SampleType> context) {}),
      process_hpf ([] (juce::dsp::ProcessContextReplacing<SampleType> context) {})
{
    lpf.state->type = juce::dsp::StateVariableFilter::Parameters<SampleType>::Type::lowPass;
    hpf.state->type = juce::dsp::StateVariableFilter::Parameters<SampleType>::Type::highPass;
}

//==============================================================================
template <typename SampleType>
void CrossoverWithBuffer<SampleType>::setCutOffFrequency (SampleType newCutOffFrequency)
{
    cutOffFrequency = newCutOffFrequency;
    update();
}

template <typename SampleType>
void CrossoverWithBuffer<SampleType>::setResonance (SampleType newResonance)
{
    resonance = newResonance;
    update();
}

template <typename SampleType>
void CrossoverWithBuffer<SampleType>::setMix (SampleType newMix)
{
    dryWet.setWetMixProportion (newMix);
}

template <typename SampleType>
void CrossoverWithBuffer<SampleType>::setBalance (SampleType newBalance)
{
    balance = juce::jmin (juce::jmax (newBalance, (SampleType) 0), (SampleType) 1);
}

template <typename SampleType>
void CrossoverWithBuffer<SampleType>::setLPFProcess (std::function<void (juce::dsp::ProcessContextReplacing<SampleType>)> f)
{
    process_lpf = f;
}

template <typename SampleType>
void CrossoverWithBuffer<SampleType>::setHPFProcess (std::function<void (juce::dsp::ProcessContextReplacing<SampleType>)> f)
{
    process_hpf = f;
}

//==============================================================================
template <typename SampleType>
void CrossoverWithBuffer<SampleType>::prepare (const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    dryWet.prepare (spec);
    lpf.prepare (spec);
    hpf.prepare (spec);
    lpfBuffer.reset (
        new juce::AudioBuffer<SampleType> (
            spec.numChannels, spec.maximumBlockSize));
    hpfBuffer.reset (
        new juce::AudioBuffer<SampleType> (
            spec.numChannels, spec.maximumBlockSize));
    update();
}

template <typename SampleType>
void CrossoverWithBuffer<SampleType>::reset()
{
    dryWet.reset();
    lpf.reset();
    hpf.reset();
}

//==============================================================================
template <typename SampleType>
void CrossoverWithBuffer<SampleType>::update()
{
    lpf.state->setCutOffFrequency (sampleRate, cutOffFrequency, resonance);
    hpf.state->setCutOffFrequency (sampleRate, cutOffFrequency, resonance);
}

//==============================================================================
template class CrossoverWithBuffer<float>;
template class CrossoverWithBuffer<double>;
