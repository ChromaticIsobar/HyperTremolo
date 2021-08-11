/*
  ==============================================================================

    ToggleWrapper.cpp

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

#include "ToggleWrapper.h"

//==============================================================================
juce::Image throughZeroImage (
    juce::Colour colour,
    int imageWidth,
    int imageHeight,
    float thickness,
    bool clearImage,
    int nCircles,
    juce::Image::PixelFormat format)
{
    juce::Image img (format, imageWidth, imageHeight, clearImage);
    juce::Graphics g (img);

    g.setColour (colour);
    for (int i = 0; i < nCircles; ++i)
    {
        float iThick = thickness / (i + 1.0f),
              effectiveWidth = imageWidth - iThick, effectiveHeight = imageHeight - iThick,
              width = effectiveWidth / (i + 1.0f), height = effectiveHeight / (i + 1.0f),
              hpad = (imageWidth - width) / 2.0f, vpad = (imageWidth - height) / 2.0f;
        g.drawEllipse (hpad, vpad, width, height, iThick);
    }

    return img;
}

juce::Image smallCircleImage (
    juce::Colour colour,
    int imageWidth,
    int imageHeight,
    float radius,
    bool clearImage,
    juce::Image::PixelFormat format)
{
    juce::Image img (format, imageWidth, imageHeight, clearImage);
    juce::Graphics g (img);

    g.setColour (colour);
    g.fillEllipse (
        imageWidth / 2.0f - radius,
        imageHeight / 2.0f - radius,
        2 * radius,
        2 * radius);

    return img;
}

//==============================================================================
ToggleWrapper::ToggleWrapper (juce::String id, bool clickingTogglesState)
    : id (id)
{
    label.setJustificationType (juce::Justification::centred);
    toggle.setClickingTogglesState (clickingTogglesState);
}

void ToggleWrapper::applyTo (juce::AudioProcessorEditor& editor,
                             juce::AudioProcessorValueTreeState& vts,
                             int labelWidth,
                             int newLabelHeight)
{
    labelHeight = newLabelHeight;
    auto param = vts.getParameter (id);
    label.setText (param->getName (32), juce::dontSendNotification);
    attachment.reset (
        new juce::AudioProcessorValueTreeState::ButtonAttachment (
            vts, id, toggle));

    editor.addAndMakeVisible (label);
    editor.addAndMakeVisible (toggle);
}

void ToggleWrapper::setBounds (juce::Rectangle<int>& newBounds)
{
    newBounds.removeFromBottom (labelHeight / 2);
    label.setBounds (newBounds.removeFromBottom (labelHeight));

    // Get image dimensions
    int imgWidth = juce::jmax (
        toggle.getNormalImage().getWidth(),
        juce::jmax (
            toggle.getOverImage().getWidth(),
            toggle.getDownImage().getWidth()));
    int imgHeight = juce::jmax (
        toggle.getNormalImage().getHeight(),
        juce::jmax (
            toggle.getOverImage().getHeight(),
            toggle.getDownImage().getHeight()));

    // Crop away extra space
    int hpad = juce::jmax (0, newBounds.getWidth() - imgWidth) / 2;
    int tpad = juce::jmax (0, newBounds.getHeight() - labelHeight / 2 - imgHeight) / 2;
    int bpad = juce::jmax (0, newBounds.getHeight() - tpad - imgHeight);

    newBounds.removeFromLeft (hpad);
    newBounds.removeFromRight (hpad);
    newBounds.removeFromTop (tpad);
    newBounds.removeFromBottom (bpad);

    toggle.setBounds (newBounds);
}

void ToggleWrapper::setImages (bool resizeButtonNowToFitThisImage,
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
                               float hitTestAlphaThreshold)
{
    toggle.setImages (resizeButtonNowToFitThisImage,
                      rescaleImagesWhenButtonSizeChanges,
                      preserveImageProportions,
                      normalImage,
                      imageOpacityWhenNormal,
                      overlayColourWhenNormal,
                      overImage,
                      imageOpacityWhenOver,
                      overlayColourWhenOver,
                      downImage,
                      imageOpacityWhenDown,
                      overlayColourWhenDown,
                      hitTestAlphaThreshold);
}

void ToggleWrapper::setOnClick (std::function<void()> f)
{
    toggle.onClick = f;
}

void ToggleWrapper::setTooltip (const juce::String& newToolTip)
{
    toggle.setTooltip (newToolTip);
}
