/*
  ==============================================================================

    Editor.cpp

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

#include "Editor.h"

//==============================================================================
HyperTremoloPluginEditor::HyperTremoloPluginEditor (juce::AudioProcessor& p,
                                                    juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), valueTreeState (vts),
    mixKnob ("mix"), gainKnob ("gain"), tremZeroToggle ("tremZero"), tremRateKnob ("tremRate"),
    xoverFreqKnob ("xoverFreq"), xoverResonKnob ("xoverReson"), xoverBalanceKnob ("xoverBalance")
{
    // Set up through-zero toggle images
    auto offColour = getLookAndFeel().findColour (juce::Slider::ColourIds::rotarySliderFillColourId);
    auto onColour = getLookAndFeel().findColour (juce::Slider::ColourIds::thumbColourId);
    auto imgWidth = knobWidth - knobLabelHeight - knobMatrixColSep;
    float lineThick = 8.0f;
    auto onImg = throughZeroImage (onColour, imgWidth, imgWidth, lineThick);
    auto offImg = throughZeroImage (offColour, imgWidth, imgWidth, lineThick);

    tremZeroToggle.setImages (
        true, false, true, offImg, 1.0f, {}, onImg, 0.666f, {}, onImg, 1.0f, {}, 0.9f);

    // Apply controls to the editor
    mixKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    gainKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    tremZeroToggle.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    tremRateKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    xoverFreqKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    xoverResonKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    xoverBalanceKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);

    setSize(knobMatrixWidth, knobMatrixHeight);
}

//==============================================================================
void HyperTremoloPluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void HyperTremoloPluginEditor::resized()
{
    auto rect = getLocalBounds();
    auto row = rect.removeFromTop (knobHeight + knobLabelHeight);

    tremZeroToggle.setBounds (row.removeFromLeft (knobWidth));
    row.removeFromLeft (knobMatrixColSep);
    tremRateKnob.setBounds (row.removeFromLeft (knobWidth));

    row = rect.removeFromTop (knobMatrixRowSep);
    row = rect.removeFromTop (knobHeight + knobLabelHeight);

    xoverFreqKnob.setBounds (row.removeFromLeft (knobWidth));
    row.removeFromLeft (knobMatrixColSep);
    xoverResonKnob.setBounds (row.removeFromLeft (knobWidth));
    row.removeFromLeft (knobMatrixColSep);
    xoverBalanceKnob.setBounds (row.removeFromLeft (knobWidth));

    row = rect.removeFromTop (knobMatrixRowSep);
    row = rect.removeFromTop (knobHeight + knobLabelHeight);

    gainKnob.setBounds (row.removeFromLeft (knobWidth));
    row.removeFromLeft (knobMatrixColSep);
    mixKnob.setBounds (row.removeFromLeft (knobWidth));
}
