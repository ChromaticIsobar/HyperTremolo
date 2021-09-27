/*
  ==============================================================================

    DualTremolo.cpp

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

#include "DualTremolo.h"

//==============================================================================
template <typename SampleType>
DualTremolo<SampleType>::DualTremolo()
{
}

//==============================================================================
template <typename SampleType>
void DualTremolo<SampleType>::setTremoloRate (SampleType f)
{
    rate = f;
    update();
}

template <typename SampleType>
void DualTremolo<SampleType>::setTremoloRatio (SampleType f)
{
    ratio = f;
    update();
}

template <typename SampleType>
void DualTremolo<SampleType>::setTremoloPhase (SampleType f)
{
    phase = f;
}

//==============================================================================
template <typename SampleType>
void DualTremolo<SampleType>::sync()
{
    DBG ("Syncing DualTremolo:");
    DBG ("  Current phases:           " << lpfTrem.getPhase() << " (L) " << hpfTrem.getPhase() << " (H)");
    DBG ("  Current phase difference: " << std::fmod (
        juce::MathConstants<SampleType>::twoPi + hpfTrem.getPhase() - lpfTrem.getPhase(),
        juce::MathConstants<SampleType>::twoPi));
    auto p = juce::MathConstants<SampleType>::twoPi + phase;
    p += lpfTrem.getPhase() - hpfTrem.getPhase();
    DBG ("  Advancing HPF tremolo by: " << p);
    hpfTrem.advance (p);
    DBG ("  New phases:               " << lpfTrem.getPhase() << " (L) " << hpfTrem.getPhase() << " (H)");
    DBG ("  New phase difference:     " << std::fmod (
             juce::MathConstants<SampleType>::twoPi + hpfTrem.getPhase() - lpfTrem.getPhase(),
             juce::MathConstants<SampleType>::twoPi));
    DBG ("  Target phase difference:  " << phase);
}

//==============================================================================
template <typename SampleType>
void DualTremolo<SampleType>::update()
{
    lpfTrem.setRate (rate);
    hpfTrem.setRate (rate * ratio);
}

//==============================================================================
template class DualTremolo<float>;
template class DualTremolo<double>;
