/*
  ==============================================================================

    PhaseControlledOscillator.h

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

/** Wave shapes for oscillator */
enum class PhaseControlledOscillatorWaveShape
{
    sine,
    sawtooth
};

/**
    Oscillator which can be controlled with an external phase

    @tags{DSP}
*/
template <typename SampleType>
class PhaseControlledOscillator
{
public:
    //==============================================================================
    /** Constructor. */
    PhaseControlledOscillator();

    /** Sets the wave shape function of the oscillator. */
    void setShape (std::function<SampleType (SampleType)>);

    /** Sets the wave shape function of the oscillator. */
    void setShape (PhaseControlledOscillatorWaveShape);

    /** Call the approximated wave shape function */
    SampleType waveShapeFunc (SampleType);

    /** Sets the rate (in Hz) of the oscillator */
    void setRate (SampleType);

    //==============================================================================
    /** Gets the current phase of the oscillator.
            If a delay in samples is specified, then it gets the phase of the
            oscillator after said delay
        */
    SampleType getPhase (size_t delay = 0);

    //==============================================================================
    /** Initialises the oscillator. */
    void prepare (const juce::dsp::ProcessSpec&);

    /** Resets the internal state variables of the oscillator. */
    void reset();

    /** Advances the oscillator by the given phase amount. */
    void advance (SampleType);

    /** Sets the phase offset target value */
    void setOffset (SampleType);

    /** Sets the phase offset target and current value */
    void setOffset (SampleType, SampleType);

    /** Gets the phase offset target value */
    SampleType getOffset();

    //==============================================================================
    /** Writes the output samples for the oscillator waveshape. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto& outputBlock = context.getOutputBlock();
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples = outputBlock.getNumSamples();

        if (context.isBypassed)
        {
            outputBlock.fill ((SampleType) 0);
            return;
        }

        if (numChannels > 0)
        {
            auto* outputSamples = outputBlock.getChannelPointer (0);

            for (size_t i = 0; i < numSamples; ++i)
                outputSamples[i] = waveShapeFunc (getPhase (i) + offset.getNextValue());
            auto firstChannelBlock = outputBlock.getSingleChannelBlock (0);
            for (size_t channel = 1; channel < numChannels; ++channel)
                outputBlock.getSingleChannelBlock (channel).copyFrom (firstChannelBlock);
        }
        advance (samplesToPhase (numSamples));
    }

private:
    //==============================================================================
    /** Convert a value in samples to the corresponding oscillator phase */
    SampleType samplesToPhase (size_t);

    //==============================================================================
    const size_t lookup_size = 64;
    juce::dsp::LookupTableTransform<SampleType> lookup;

    SampleType sampleRate = 44100.0;
    // Phase derivative (not frequency)
    SampleType rate = 1.0;

    //==============================================================================
    juce::dsp::Phase<SampleType> phase;
    juce::SmoothedValue<SampleType, juce::ValueSmoothingTypes::Linear> offset;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PhaseControlledOscillator<SampleType>)
};
