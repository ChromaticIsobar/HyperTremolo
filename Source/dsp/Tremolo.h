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

#include "PhaseControlledOscillator.h"
#include <JuceHeader.h>

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
    void setShape (PhaseControlledOscillatorWaveShape waveShape);

    /** Sets the amount of dry and wet signal in the output of the
        tremolo (between 0 for full dry and 1 for full wet).
    */
    void setMix (SampleType newMix);

    /** Sets the phase offset target value */
    void setOffset (SampleType);

    /** Sets the phase offset target and current value */
    void setOffset (SampleType, SampleType);

    /** Gets the phase offset target value */
    SampleType getOffset();

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

        jassert (inputBlock.getNumChannels() == outputBlock.getNumChannels());
        jassert (inputBlock.getNumSamples() == outputBlock.getNumSamples());

        if (context.isBypassed)
        {
            outputBlock.copyFrom (inputBlock);
            return;
        }

        dryWet.pushDrySamples (inputBlock);

        juce::dsp::AudioBlock<SampleType> amBlock (*amBuffer);
        juce::dsp::ProcessContextReplacing<SampleType> amPC (amBlock);
        lfo.process (amPC);
        amScale.process (amPC);
        amBias.process (amPC);
        outputBlock.replaceWithProductOf (inputBlock, amBlock);

        dryWet.mixWetSamples (outputBlock);
    }

private:
    //==============================================================================
    juce::dsp::DryWetMixer<SampleType> dryWet;
    juce::dsp::Gain<SampleType> amScale;
    juce::dsp::Bias<SampleType> amBias;
    PhaseControlledOscillator<SampleType> lfo;
    std::unique_ptr<juce::AudioBuffer<SampleType>> amBuffer;
    SampleType rampLength = static_cast<SampleType> (0.005);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tremolo<SampleType>)
};
