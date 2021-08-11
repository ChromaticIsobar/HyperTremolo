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
