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
HyperTremoloPluginEditor::HyperTremoloPluginEditor (HyperTremoloPlugin& p,
                                                    juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), valueTreeState (vts), mixKnob ("mix"), gainKnob ("gain"),
    tremZeroToggle ("tremZero"), tremRateKnob ("tremRate"), tremRatioKnob ("tremRatio"), tremMixKnob ("tremMix"),
    xoverFreqKnob ("xoverFreq"), xoverResonKnob ("xoverReson"), xoverBalanceKnob ("xoverBalance"), xoverMixKnob ("xoverMix"),
    tremSyncButton ("tremSync", false)
{
    // Set up button images
    auto offColour = getLookAndFeel().findColour (juce::Slider::ColourIds::rotarySliderFillColourId);
    auto onColour = getLookAndFeel().findColour (juce::Slider::ColourIds::thumbColourId);
    auto imgWidth = knobWidth - knobLabelHeight - knobMatrixColSep;
    float lineThick = 8.0f;
    auto onImg = throughZeroImage (onColour, imgWidth, imgWidth, lineThick);
    auto offImg = throughZeroImage (offColour, imgWidth, imgWidth, lineThick);

    tremZeroToggle.setImages (
        true, false, true, offImg, 1.0f, {}, onImg, 0.333f, {}, onImg, 1.0f, {}, 0.9f);

    lineThick *= 0.75f;
    onImg = smallCircleImage (onColour, imgWidth, imgWidth, lineThick);
    offImg = smallCircleImage (offColour, imgWidth, imgWidth, lineThick);

    tremSyncButton.setImages (
        true, false, true, offImg, 1.0f, {}, onImg, 0.333f, {}, onImg, 1.0f, {}, 0.9f);
    tremSyncButton.setOnClick (std::bind (&HyperTremoloPlugin::sync, &p));
    addAndMakeVisible (tooltipWindow);
    tremSyncButton.setTooltip ("Sync the two tremolos. Click\nthis when setting ratio to 1");

    // Apply controls to the editor
    mixKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    gainKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    tremZeroToggle.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    tremRateKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    tremRatioKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    tremSyncButton.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    tremMixKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    xoverFreqKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    xoverResonKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    xoverBalanceKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);
    xoverMixKnob.applyTo (*this, valueTreeState, knobWidth, knobLabelHeight);

    setSize(knobMatrixWidth, knobMatrixHeight);
}

//==============================================================================
template <typename ValueType>
void drawRoundedRectangleHelper (juce::Graphics& g,
                                 juce::Rectangle<ValueType> r,
                                 float cornerSize = 1.0f,
                                 float lineThickness = 1.0f)
{
    g.fillRoundedRectangle (r.getX() + lineThickness / 2.0f,
                            r.getY() + lineThickness / 2.0f,
                            r.getWidth() - lineThickness,
                            r.getHeight() - lineThickness,
                            cornerSize);
}

void HyperTremoloPluginEditor::paint (juce::Graphics& g)
{
    // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    // g.setColour (getLookAndFeel().findColour (juce::Slider::ColourIds::rotarySliderFillColourId));
    g.fillAll (getLookAndFeel().findColour (juce::Slider::ColourIds::rotarySliderFillColourId));
    g.setColour (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    drawRoundedRectangleHelper (g, mixRect, 15.0f, 2.0f);
    drawRoundedRectangleHelper (g, tremRect, 15.0f, 2.0f);
    drawRoundedRectangleHelper (g, xoverRect, 15.0f, 2.0f);
}

template <typename ValueType>
void rectTrimHelper(juce::Rectangle<ValueType>& rect, ValueType hpad, ValueType vpad)
{
    rect.removeFromTop (vpad);
    rect.removeFromBottom (vpad);
    rect.removeFromLeft (hpad);
    rect.removeFromRight (hpad);
}

void HyperTremoloPluginEditor::resized()
{
    auto rect = getLocalBounds();
    
    // Group rectangles
    rectTrimHelper (rect, knobGroupColSep, knobGroupRowSep);
    mixRect = rect.removeFromRight (knobWidth + knobMatrixColSep);
    rect.removeFromRight (knobGroupColSep);
    tremRect = rect.removeFromTop (knobHeight + knobLabelHeight + 2 * knobMatrixRowSep);
    rect.removeFromTop (knobGroupRowSep);
    xoverRect = rect.removeFromTop (knobHeight + knobLabelHeight + 2 * knobMatrixRowSep);

    // Mixer
    juce::Rectangle<int> mixRectConsumable (mixRect);
    rectTrimHelper (mixRectConsumable, knobMatrixColSep, knobMatrixRowSep);
    gainKnob.setBounds (mixRectConsumable.removeFromTop (knobHeight + knobLabelHeight));
    mixRectConsumable.removeFromTop (knobMatrixRowSep * 2 + knobGroupColSep);
    mixKnob.setBounds (mixRectConsumable.removeFromTop (knobHeight + knobLabelHeight));

    // Tremolo
    juce::Rectangle<int> tremRectConsumable (tremRect);
    rectTrimHelper (tremRectConsumable, knobMatrixColSep, knobMatrixRowSep);
    tremZeroToggle.setBounds (tremRectConsumable.removeFromLeft (knobWidth));
    tremRectConsumable.removeFromLeft (2 * knobMatrixColSep);
    tremRateKnob.setBounds (tremRectConsumable.removeFromLeft (knobWidth));
    tremRectConsumable.removeFromLeft (2 * knobMatrixColSep);

    auto ratioBounds = tremRectConsumable.removeFromLeft (knobWidth);
    tremSyncButton.setBounds (juce::Rectangle<int> (ratioBounds));
    tremRatioKnob.setBounds (ratioBounds);

    tremRectConsumable.removeFromLeft (2 * knobMatrixColSep);
    tremMixKnob.setBounds (tremRectConsumable.removeFromLeft (knobWidth));

    // Crossover
    juce::Rectangle<int> xoverRectConsumable (xoverRect);
    rectTrimHelper (xoverRectConsumable, knobMatrixColSep, knobMatrixRowSep);
    xoverFreqKnob.setBounds (xoverRectConsumable.removeFromLeft (knobWidth));
    xoverRectConsumable.removeFromLeft (2 * knobMatrixColSep);
    xoverResonKnob.setBounds (xoverRectConsumable.removeFromLeft (knobWidth));
    xoverRectConsumable.removeFromLeft (2 * knobMatrixColSep);
    xoverBalanceKnob.setBounds (xoverRectConsumable.removeFromLeft (knobWidth));
    xoverRectConsumable.removeFromLeft (2 * knobMatrixColSep);
    xoverMixKnob.setBounds (xoverRectConsumable.removeFromLeft (knobWidth));
}
