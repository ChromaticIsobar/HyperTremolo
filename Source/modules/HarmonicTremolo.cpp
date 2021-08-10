/*
  ==============================================================================

    HarmonicTremolo.cpp

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

#include "HarmonicTremolo.h"
//==============================================================================
template <typename SampleType>
HarmonicTremolo<SampleType>::HarmonicTremolo()
{
}

//==============================================================================
template <typename SampleType>
void HarmonicTremolo<SampleType>::setCrossoverFrequency (SampleType f)
{
    crossover.setCutOffFrequency (f);
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::setCrossoverResonance (SampleType f)
{
    crossover.setResonance (f);
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::setCrossoverMix (SampleType f)
{
    crossover.setMix (f);
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::setCrossoverBalance (SampleType f)
{
    crossover.setBalance (f);
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::setTremoloRate (SampleType f)
{
    lpfTrem.setRate (f);
    hpfTrem.setRate (f);
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::setTremoloThroughZero (bool f)
{
    lpfTrem.setThroughZero (f);
    hpfTrem.setThroughZero (f);
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::setTremoloShape (std::function<SampleType (SampleType)> f)
{
    lpfTrem.setShape (f);
    hpfTrem.setShape (f);
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::setTremoloShape (TremoloWaveShape f)
{
    lpfTrem.setShape (f);
    hpfTrem.setShape (f);
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::setTremoloMix (SampleType f)
{
    lpfTrem.setMix (f);
    hpfTrem.setMix (f);
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::setMix (SampleType f)
{
    dryWet.setWetMixProportion (f);
}

//==============================================================================
template <typename SampleType>
void HarmonicTremolo<SampleType>::prepare (const juce::dsp::ProcessSpec& spec)
{
    dryWet.prepare (spec);
    crossover.prepare (spec);
    lpfTrem.prepare (spec);
    hpfTrem.prepare (spec);
    crossover.setLPFProcess (std::bind (
        &Tremolo<SampleType>::process<juce::dsp::ProcessContextReplacing<SampleType>>,
        &lpfTrem,
        std::placeholders::_1));
    crossover.setHPFProcess (std::bind (
        &Tremolo<SampleType>::process<juce::dsp::ProcessContextReplacing<SampleType>>,
        &hpfTrem,
        std::placeholders::_1));
}

template <typename SampleType>
void HarmonicTremolo<SampleType>::reset()
{
    dryWet.reset();
    crossover.reset();
    lpfTrem.reset();
    hpfTrem.reset();
    hpfTrem.advance (juce::MathConstants<SampleType>::halfPi);
}

//==============================================================================
template class HarmonicTremolo<float>;
template class HarmonicTremolo<double>;
