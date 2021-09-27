/*
  ==============================================================================

    DualTremolo.h

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
