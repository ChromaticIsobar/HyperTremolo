/*
  ==============================================================================

    ToggleWrapper.h

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

/** Produces an image for the through-zero toggle */
juce::Image throughZeroImage (
    juce::Colour colour = juce::Colours::black,
    int imageWidth = 32,
    int imageHeight = 32,
    float thickness = 3.0f,
    bool clearImage = true,
    int nCircles = 3,
    juce::Image::PixelFormat format = juce::Image::ARGB);

/** Produces an image with a small circle */
juce::Image smallCircleImage (
    juce::Colour colour = juce::Colours::black,
    int imageWidth = 32,
    int imageHeight = 32,
    float radius = 3.0f,
    bool clearImage = true,
    juce::Image::PixelFormat format = juce::Image::ARGB);

/**
    Wrapper class for a toggle, its label and ValueTreeState attachment
*/
class ToggleWrapper
{
public:
    //==============================================================================
    /** Constructor. */
    ToggleWrapper (juce::String id, bool clickingTogglesState = true);

    /** Apply a knob onto a processor editor. */
    void applyTo (juce::AudioProcessorEditor&,
                  juce::AudioProcessorValueTreeState&,
                  int labelWidth,
                  int newLabelHeight);

    /** Set the bounding boxes of the slider and label. */
    void setBounds (juce::Rectangle<int>&);

    /** Sets up the images to draw in various states. */
    void setImages (bool resizeButtonNowToFitThisImage,
                    bool rescaleImagesWhenButtonSizeChanges,
                    bool preserveImageProportions,
                    const juce::Image& normalImage,
                    float imageOpacityWhenNormal,
                    juce::Colour overlayColourWhenNormal,
                    const juce::Image& overImage,
                    float imageOpacityWhenOver,
                    juce::Colour overlayColourWhenOver,
                    const juce::Image& downImage,
                    float imageOpacityWhenDown,
                    juce::Colour overlayColourWhenDown,
                    float hitTestAlphaThreshold = 0.0f);

    /** Sets the callback function. */
    void setOnClick (std::function<void()>);

    /** Sets the tooltip for the button.  */
    void setTooltip (const juce::String&);

private:
    //==============================================================================
    int labelHeight = 16;
    juce::String id;
    juce::Label label;
    juce::ImageButton toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
};
