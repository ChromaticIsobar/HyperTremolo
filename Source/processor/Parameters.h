/*
  ==============================================================================

    Parameters.h

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

template <typename ValueType>
class LogRange  : public juce::NormalisableRange<ValueType>
{
public:
    LogRange (
        ValueType rangeStart,
        ValueType rangeEnd,
        ValueType intervalValue)
        : juce::NormalisableRange<ValueType> (rangeStart, rangeEnd, intervalValue, ((ValueType) 1) / std::log2 (((ValueType) 1) + std::sqrt (rangeEnd / rangeStart)))
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LogRange<ValueType>)
};

/**
    A simple listener for setting values from an AudioProcessorValueTreeState
*/
class SetterListener  : public juce::AudioProcessorValueTreeState::Listener
{
public:
    SetterListener (std::function<void (float)>);
    void parameterChanged (const juce::String&, float) override;

private:
    std::function<void (float)> setterFunction;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SetterListener)
};

/**
    A listener for setting values of AM Frequency and Through-0
*/
class ThroughZeroAndFrequencySetterListener  : public juce::AudioProcessorValueTreeState::Listener
{
public:
    ThroughZeroAndFrequencySetterListener (std::function<void (float)> frequencySetter, std::function<void (bool)> throughZeroSetter, std::function<std::atomic<float>*(juce::StringRef)> vtsGetter, juce::String freqID, juce::String tzeroID);
    void parameterChanged (const juce::String&, float) override;

private:
    juce::String frequencyParameterID, throughZeroParameterID;
    std::function<void (float)> frequencySetterFunction, throughZeroSetterFunction;
    std::function<std::atomic<float>*(juce::StringRef)> valueTreeStateGetterFunction;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThroughZeroAndFrequencySetterListener)
};
