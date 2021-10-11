/*
  ==============================================================================

    Editor.h

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

#include "../processor/Plugin.h"
#include "ToggleWrapper.h"
#include "KnobWrapper.h"
#include <JuceHeader.h>

//==============================================================================
/**
    Editor for the HyperTremolo plugin
*/
class HyperTremoloPluginEditor  : public juce::AudioProcessorEditor
{
public:
    HyperTremoloPluginEditor (HyperTremoloPlugin&,
                              juce::AudioProcessorValueTreeState&);

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    static const int knobMatrixCols = 5, knobMatrixRows = 2,
                     knobMatrixRowSep = 3, knobMatrixColSep = 2,
                     knobWidth = 78, knobLabelHeight = 13,
                     knobHeight = knobWidth + knobLabelHeight,
                     nRowGroups = 2, nColGroups = 2, 
                     knobGroupRowSep = 2, knobGroupColSep = 2,
                     footerHeight = 12,
                     knobMatrixHeight = knobMatrixRows * (knobHeight + knobLabelHeight + 2 * knobMatrixRowSep) + (nRowGroups + 1) * knobGroupRowSep,
                     knobMatrixWidth = knobMatrixCols * (knobWidth + 2 * knobMatrixColSep) + (nColGroups + 1) * knobGroupColSep;

    //==============================================================================
    KnobWrapper mixKnob, gainKnob, tremRateKnob, tremRatioKnob, tremMixKnob,
        xoverFreqKnob, xoverResonKnob, xoverBalanceKnob, xoverMixKnob;
    ToggleWrapper tremZeroToggle, tremSyncButton;
    juce::TooltipWindow tooltipWindow;
    juce::Rectangle<int> tremRect, xoverRect, mixRect, footerRect;
    juce::Label footerLeftLabel, footerRightLabel;
    juce::String footerLeftString, footerRightString;

    //==============================================================================
    juce::AudioProcessorValueTreeState& valueTreeState;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HyperTremoloPluginEditor)
};
