/*
  ==============================================================================

    Tremolo.h

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

/** Wave shape for amplitude modulation LFO modulant */
enum class TremoloWaveShape
{
    sine,
    sawtooth
};

/**
    A simple amplitude modulation DSP widget.

    This audio effect can be controlled via the rate and shape
    of the modulant LFO and a mix control.

    This widget also allows explicit control of the modulant phase. This
    is useful for syncronizing several effects.

    @tags{DSP}
*/
template <typename SampleType>
class Tremolo
{
public:
    //==============================================================================
    /** Constructor. */
    Tremolo();

    //==============================================================================
    /** Sets the rate (in Hz) of the modulant LFO. */
    void setRate (SampleType newRateHz);

    /** Sets the AM to be through zero or not. */
    void setThroughZero (bool);

    /** Sets the wave shape function of the modulant LFO. */
    void setShape (std::function<SampleType (SampleType)>);

    /** Sets the shape of the modulant LFO. */
    void setShape (TremoloWaveShape waveShape);

    /** Sets the amount of dry and wet signal in the output of the
        tremolo (between 0 for full dry and 1 for full wet).
    */
    void setMix (SampleType newMix);

    //==============================================================================
    /** Gets the current phase of the modulant LFO. */
    SampleType getPhase();

    //==============================================================================
    /** Initialises the processor. */
    void prepare (const juce::dsp::ProcessSpec&);

    /** Resets the internal state variables of the processor. */
    void reset();

    /** Advances the modulant LFO by the given phase amount. */
    void advance (SampleType);

    //==============================================================================
    /** Processes the input and output samples supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples = outputBlock.getNumSamples();

        jassert (inputBlock.getNumChannels() == numChannels);
        jassert (inputBlock.getNumSamples() == numSamples);

        if (context.isBypassed)
        {
            outputBlock.copyFrom (inputBlock);
            return;
        }

        dryWet.pushDrySamples (inputBlock);

        SampleType a = 0.0;
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples = inputBlock.getChannelPointer (channel);
            auto* outputSamples = outputBlock.getChannelPointer (channel);

            for (size_t i = 0; i < numSamples; ++i)
            {
                a = waveShapeFunc (phaseAfter (i));
                if (throughZero)
                    a = ((SampleType) 2) * a - ((SampleType) 1);
                outputSamples[i] = inputSamples[i] * a;
            }
        }
        advance (samplesToPhase (numSamples));

        dryWet.mixWetSamples (outputBlock);
    }

private:
    //==============================================================================
    /** Get the LFO phase after a given delay (in samples) */
    SampleType phaseAfter (size_t);

    /** Convert a value in samples to the corresponding LFO phase */
    SampleType samplesToPhase (size_t);

    //==============================================================================
    juce::dsp::DryWetMixer<SampleType> dryWet;
    juce::dsp::Phase<SampleType> phase;

    //==============================================================================
    std::function<SampleType (SampleType)> waveShapeFunc;
    SampleType sampleRate = 44100.0, rate = 1.0;
    bool throughZero = false;
};
