/*
  ==============================================================================

    CrossoverWithBuffer.h
    
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

/**
    A wrapper for a pair of filters (a low-pass and a high-pass) and process
    functions, each one dedicated to its own band. 

    Calling the process method applies the two filters on the input signals. 
    On each of the two signals, the respective process function is called. 
    Finally, the two signals are mixed abck together. 

    The intermediate signals are stored in internal buffers.

    @tags{DSP}
*/
template <typename SampleType>
class CrossoverWithBuffer
{
public:
    //==============================================================================
    /** Constructor. */
    CrossoverWithBuffer();

    //==============================================================================
    /** Sets the cutoff frequency (in Hz) of the filters. */
    void setCutOffFrequency (SampleType newCutOffFrequency);

    /** Sets the resonance of the filters. */
    void setResonance (SampleType newResonance);

    /** Sets the amount of dry and wet signal in the output of the
        filter (between 0 for full dry and 1 for full wet).
    */
    void setMix (SampleType newMix);

    /** Sets the amount of low-pass and high-pass in the output of the
        filter (between 0 for full low-pass and 1 for full high-pass).
    */
    void setBalance (SampleType newBalance);

    /** Sets the process function for the LPF signal */
    void setLPFProcess (std::function<void (juce::dsp::ProcessContextReplacing<SampleType>)>);

    /** Sets the process function for the HPF signal */
    void setHPFProcess (std::function<void (juce::dsp::ProcessContextReplacing<SampleType>)>);

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

        juce::dsp::AudioBlock<float> lpfBlock (*lpfBuffer);
        jassert (lpfBlock.getNumSamples() >= numSamples);

        // Process LPF path (non-replacing)
        dryWet.pushDrySamples (inputBlock);
        lpf.process (juce::dsp::ProcessContextNonReplacing<SampleType> (inputBlock, lpfBlock));
        dryWet.mixWetSamples (lpfBlock);

        // Process HPF path (replacing)
        dryWet.pushDrySamples (inputBlock);
        hpf.process (context);
        dryWet.mixWetSamples (outputBlock);

        // Apply band-wise process functions
        process_lpf (juce::dsp::ProcessContextReplacing<SampleType> (lpfBlock));
        process_hpf (juce::dsp::ProcessContextReplacing<SampleType> (outputBlock));

        // Mix signals
        balance.pushDrySamples (lpfBlock);
        balance.mixWetSamples (outputBlock);
    }

private:
    //==============================================================================
    juce::dsp::StateVariableTPTFilter<SampleType> lpf, hpf;
    juce::dsp::DryWetMixer<SampleType> dryWet, balance;

    //==============================================================================
    std::unique_ptr<juce::AudioBuffer<SampleType>> lpfBuffer;

    //==============================================================================
    double sampleRate = 44100.0;
    std::function<void (juce::dsp::ProcessContextReplacing<SampleType>)> process_lpf,
        process_hpf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CrossoverWithBuffer<SampleType>)
};
