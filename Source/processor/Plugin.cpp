/*
  ==============================================================================

    Plugin.cpp

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

#include "../editor/Editor.h"
#include "Parameters.h"
#include "Plugin.h"

//==============================================================================
HyperTremoloPlugin::HyperTremoloPlugin()
    :
#ifndef JucePlugin_PreferredChannelConfigurations
      AudioProcessor (
          BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
              .withInput ("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
#endif
      valueTreeState (*this, nullptr, "HyperTremolo", { std::make_unique<juce::AudioParameterFloat> ("gain", "Makeup Gain", juce::NormalisableRange<float> (-20.0f, 20.0f, 0.01f), 0.0f, "dB"), std::make_unique<juce::AudioParameterFloat> ("mix", "Mix", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f), std::make_unique<juce::AudioParameterFloat> ("tremRate", "Rate", juce::NormalisableRange<float> (0.0f, 20.0f, 0.0001f, 0.333f), 2.5f, "Hz"), std::make_unique<juce::AudioParameterFloat> ("xoverFreq", "Crossover", FrequencyRange<float> (112.5f, 20000.0f, 0.01f), 1500.0f, "Hz"), std::make_unique<juce::AudioParameterFloat> ("xoverReson", "Resonance", FrequencyRange<float> (0.125f, 4.0f, 0.001f), 1.0f / juce::MathConstants<float>::sqrt2), std::make_unique<juce::AudioParameterFloat> ("xoverBalance", "Balance", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.5f) })
{
}

//==============================================================================
const juce::String HyperTremoloPlugin::getName() const
{
    return JucePlugin_Name;
}

bool HyperTremoloPlugin::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool HyperTremoloPlugin::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool HyperTremoloPlugin::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double HyperTremoloPlugin::getTailLengthSeconds() const
{
    return 0.0;
}

int HyperTremoloPlugin::getNumPrograms()
{
    return 1;
}

int HyperTremoloPlugin::getCurrentProgram()
{
    return 0;
}

void HyperTremoloPlugin::setCurrentProgram (int index)
{
}

const juce::String HyperTremoloPlugin::getProgramName (int index)
{
    return {};
}

void HyperTremoloPlugin::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HyperTremoloPlugin::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    gain.prepare (spec);
    dryWet.prepare (spec);
    processor.prepare (spec);
}

void HyperTremoloPlugin::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HyperTremoloPlugin::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void HyperTremoloPlugin::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);

    update();
    dryWet.pushDrySamples (context.getInputBlock());
    processor.process (context);
    gain.process (context);
    dryWet.mixWetSamples (context.getOutputBlock());
}

//==============================================================================
bool HyperTremoloPlugin::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* HyperTremoloPlugin::createEditor()
{
    return new HyperTremoloPluginEditor(*this, valueTreeState);
}

//==============================================================================
void HyperTremoloPlugin::update()
{
    dryWet.setWetMixProportion (*valueTreeState.getRawParameterValue ("mix"));
    gain.setGainDecibels (*valueTreeState.getRawParameterValue ("gain"));
    processor.setTremoloRate (*valueTreeState.getRawParameterValue ("tremRate"));
    processor.setCrossoverFrequency (*valueTreeState.getRawParameterValue ("xoverFreq"));
    processor.setCrossoverResonance (*valueTreeState.getRawParameterValue ("xoverReson"));
    processor.setCrossoverBalance (*valueTreeState.getRawParameterValue ("xoverBalance"));
}

void HyperTremoloPlugin::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void HyperTremoloPlugin::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (valueTreeState.state.getType()))
            valueTreeState.replaceState (juce::ValueTree::fromXml (*xmlState));
}
