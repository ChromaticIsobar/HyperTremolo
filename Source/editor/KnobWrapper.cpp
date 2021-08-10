/*
  ==============================================================================

    KnobWrapper.cpp

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

#include "KnobWrapper.h"

//==============================================================================
KnobWrapper::KnobWrapper (juce::String id)
    : id (id)
{
    slider.setSliderStyle (juce::Slider::Rotary);
    label.setJustificationType (juce::Justification::centred);
}

void KnobWrapper::applyTo (juce::AudioProcessorEditor& editor,
                           juce::AudioProcessorValueTreeState& vts,
                           int labelWidth,
                           int newLabelHeight)
{
    labelHeight = newLabelHeight;
    auto param = vts.getParameter (id);
    label.setText (param->getName (32), juce::dontSendNotification);
    attachment.reset (
        new juce::AudioProcessorValueTreeState::SliderAttachment (
            vts, id, slider));
    slider.setTextValueSuffix (param->getLabel());
    slider.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::TextBoxBelow,
                            false,
                            labelWidth,
                            labelHeight);
    editor.addAndMakeVisible (label);
    editor.addAndMakeVisible (slider);
}

void KnobWrapper::setBounds (juce::Rectangle<int>& newBounds)
{
    label.setBounds (newBounds.removeFromBottom (labelHeight));
    slider.setBounds (newBounds);
}
