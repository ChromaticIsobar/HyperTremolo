/*
  ==============================================================================

    DualTremolo.h
    Created: 11 Aug 2021 12:04:04pm
    Author:  Utente

  ==============================================================================
*/

#pragma once

#include "HarmonicTremolo.h"
#include <JuceHeader.h>

template <typename SampleType>
class DualTremolo  : public HarmonicTremolo<SampleType>
{
public:
    //==============================================================================
    DualTremolo();

    //==============================================================================
    /** Sets the rate (in Hz) of the tremolo. */
    void setTremoloRate (SampleType) override;

    /** Sets the speed ratio of the two tremolos. */
    void setTremoloRatio (SampleType);

    /** Sets the phase difference of the two tremolos. */
    void setTremoloPhase (SampleType);

    //==============================================================================
    /** Syncronizes the two tremolos to be at the correct phase difference. */
    void sync();

private:
    //==============================================================================
    /** Updates the processor parameters */
    void update();

    //==============================================================================
    SampleType rate = 1.0, ratio = 1.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DualTremolo<SampleType>)
};
