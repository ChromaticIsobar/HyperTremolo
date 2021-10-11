/*
  ==============================================================================

    HarmonicTremolo.h

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

#include "CrossoverWithBuffer.h"
#include "Tremolo.h"
#include <JuceHeader.h>

/**
    A simple DSP widget that implements an Harmonic Tremolo. 

    This audio effect can be controlled via the speed, shape, and mix of the
    tremolo, and the frequency, resonance, mix and balance of the crossover filter.

    @tags{DSP}
*/
template <typename SampleType>
class HarmonicTremolo
{
public:
    //==============================================================================
    HarmonicTremolo();

    //==============================================================================
    /** Sets the cutoff frequency (in Hz) of the crossover filters. */
    void setCrossoverFrequency (SampleType);

    /** Sets the resonance of the crossover filters. */
    void setCrossoverResonance (SampleType);

    /** Sets the amount of dry and wet signal in the output of the
        crossover filter (between 0 for full dry and 1 for full wet).
    */
    void setCrossoverMix (SampleType);

    /** Sets the balance of the crossover filter (between 0 for full low-pass
        and 1 for full high-pass).
    */
    void setCrossoverBalance (SampleType);

    /** Sets the rate (in Hz) of the tremolo. */
    virtual void setTremoloRate (SampleType);

    /** Sets the tremolo to be through zero or not. */
    void setTremoloThroughZero (bool);

    /** Sets the wave shape function of the tremolo. */
    void setTremoloShape (std::function<SampleType (SampleType)>);

    /** Sets the wave shape of the tremolo. */
    void setTremoloShape (PhaseControlledOscillatorWaveShape);

    /** Sets the amount of dry and wet signal in the output of the
        tremolo (between 0 for full dry and 1 for full wet).
    */
    void setTremoloMix (SampleType);

    /** Sets the amount of dry and wet signal in the output of the
        overall effect (between 0 for full dry and 1 for full wet).
    */
    void setMix (SampleType);

    //==============================================================================
    /** Initialises the processor. */
    void prepare (const juce::dsp::ProcessSpec&);

    /** Resets the internal state variables of the processor. */
    void reset();

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
        crossover.process (context);
        dryWet.mixWetSamples (outputBlock);
    }

protected:
    //==============================================================================
    Tremolo<SampleType> lpfTrem, hpfTrem;
    SampleType phase = juce::MathConstants<SampleType>::halfPi;

private:
    //==============================================================================
    CrossoverWithBuffer<SampleType> crossover;
    juce::dsp::DryWetMixer<SampleType> dryWet;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonicTremolo<SampleType>)
};
