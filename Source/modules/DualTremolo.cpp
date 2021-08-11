/*
  ==============================================================================

    DualTremolo.cpp
    Created: 11 Aug 2021 12:04:04pm
    Author:  Utente

  ==============================================================================
*/

#include "DualTremolo.h"

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
    DBG ("  Current phases:           " << lpfTrem.getPhase() << ", " << hpfTrem.getPhase());
    DBG ("  Current phase difference: " << std::fmod (
        juce::MathConstants<SampleType>::twoPi + hpfTrem.getPhase() - lpfTrem.getPhase(),
        juce::MathConstants<SampleType>::twoPi));
    auto p = juce::MathConstants<SampleType>::twoPi + phase;
    p += lpfTrem.getPhase() - hpfTrem.getPhase();
    DBG ("  Advancing LPF tremolo by: " << p);
    hpfTrem.advance (p);
    DBG ("  New phases:               " << lpfTrem.getPhase() << ", " << hpfTrem.getPhase());
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
