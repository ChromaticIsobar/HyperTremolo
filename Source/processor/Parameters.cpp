/*
  ==============================================================================

    Parameters.cpp

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

#include "Parameters.h"

SetterListener::SetterListener (std::function<void (float)> f)
    : setterFunction (f)
{
}

void SetterListener::parameterChanged (const juce::String& id, float newValue)
{
    setterFunction (newValue);
}

//==============================================================================
ThroughZeroAndFrequencySetterListener::ThroughZeroAndFrequencySetterListener (
    std::function<void (float)> frequencySetter,
    std::function<void (bool)> throughZeroSetter,
    std::function<std::atomic<float>*(juce::StringRef)> vtsGetter,
    juce::String freqID,
    juce::String tzeroID)
    : frequencySetterFunction (frequencySetter), throughZeroSetterFunction (throughZeroSetter), valueTreeStateGetterFunction (vtsGetter), frequencyParameterID (freqID), throughZeroParameterID (tzeroID)
{
}

void ThroughZeroAndFrequencySetterListener::parameterChanged (const juce::String& id, float newValue)
{
    float freq;
    bool throughZero;
    if (id.compare (frequencyParameterID) == 0)
    {
        freq = newValue;
        throughZero = 0.0f != *valueTreeStateGetterFunction (throughZeroParameterID);
    }
    else if (id.compare (throughZeroParameterID) == 0)
    {
        freq = *valueTreeStateGetterFunction (frequencyParameterID);
        throughZero = 0.0f != newValue;
        throughZeroSetterFunction (throughZero);
    }
    else
    {
        // The listened parameter should be either the frequency
        // value or the through-0 flag
        jassertfalse;
    }
    frequencySetterFunction (freq / (throughZero + 1.0f));
}