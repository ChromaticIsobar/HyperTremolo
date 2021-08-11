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
    void setTremoloRate (SampleType) override;
    void setTremoloRatio (SampleType);

private:
    //==============================================================================
    void update();

    //==============================================================================
    SampleType rate = 1.0, ratio = 1.0;
};
