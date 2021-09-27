/*
  ==============================================================================

    KnobWrapper.h

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

#include <JuceHeader.h>

/**
    Wrapper class for a rotary slider, its label and ValueTreeState attachment
*/
class KnobWrapper
{
public:
    //==============================================================================
    /** Constructor. */
    KnobWrapper (juce::String id);

    /** Apply a knob onto a processor editor. */
    void applyTo (juce::AudioProcessorEditor&,
                  juce::AudioProcessorValueTreeState&,
                  int labelWidth,
                  int newLabelHeight);

    /** Set the bounding boxes of the slider and label */
    void setBounds (juce::Rectangle<int>&);

private:
    //==============================================================================
    int labelHeight = 16;
    juce::String id;
    juce::Label label;
    juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobWrapper)
};
