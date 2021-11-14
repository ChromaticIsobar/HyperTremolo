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

#include "Plugin.h"
#include "../editor/Editor.h"

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
      valueTreeState (*this, nullptr, "HyperTremolo", createParameterLayout())
{
    initializeListeners();
}

HyperTremoloPlugin::~HyperTremoloPlugin()
{
    auto n = listeners.size();
    for (unsigned int i = 0; i < n; ++i)
        if (listeners[i] != nullptr)
        {
            for (unsigned int j = i + 1; j < n; ++j)
                if (listeners[i] == listeners[j])
                    listeners[j] = nullptr;
            delete listeners[i];
        }
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

void HyperTremoloPlugin::setCurrentProgram (int)
{
}

const juce::String HyperTremoloPlugin::getProgramName (int)
{
    return {};
}

void HyperTremoloPlugin::changeProgramName (int, const juce::String&)
{
}

//==============================================================================
void HyperTremoloPlugin::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    gain.reset();
    dryWet.reset();
    processor.reset();

    gain.prepare (spec);
    dryWet.prepare (spec);
    processor.prepare (spec);

    // Enforce parameter value update
    sendValueChangedMessageToAllListeners (valueTreeState);
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

void HyperTremoloPlugin::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);

    dryWet.pushDrySamples (context.getInputBlock());
    processor.process (context);
    gain.process (context);
    dryWet.mixWetSamples (context.getOutputBlock());
}

void HyperTremoloPlugin::sync()
{
    processor.sync();
}

//==============================================================================
bool HyperTremoloPlugin::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* HyperTremoloPlugin::createEditor()
{
    return new HyperTremoloPluginEditor (*this, valueTreeState);
}

//==============================================================================
void HyperTremoloPlugin::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml (valueTreeState.copyState().createXml());
    copyXmlToBinary (*xml, destData);
}

void HyperTremoloPlugin::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));
    if (xml.get() != nullptr)
        if (xml->hasTagName (valueTreeState.state.getType()))
            valueTreeState.replaceState (juce::ValueTree::fromXml (*xml));
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout HyperTremoloPlugin::createParameterLayout()
{
    return {
        std::make_unique<juce::AudioParameterFloat> (
            "gain", "Gain", juce::NormalisableRange<float> (-20.0f, 20.0f, 0.01f), 0.0f, "dB"),
        std::make_unique<juce::AudioParameterFloat> (
            "mix", "Mix", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f),
        std::make_unique<juce::AudioParameterFloat> (
            "tremRate", "Rate", juce::NormalisableRange<float> (0.0f, 20.0f, 0.001f, 0.333f), 2.5f, "Hz"),
        std::make_unique<juce::AudioParameterFloat> (
            "tremRatio", "Ratio", LogRange<float> (0.1f, 10.0f, 0.001f), 1.0f),
        std::make_unique<juce::AudioParameterFloat> (
            "tremMix", "Depth", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.5f),
        std::make_unique<juce::AudioParameterBool> (
            "tremZero", "Through-0", false),
        std::make_unique<juce::AudioParameterBool> (
            "tremSync", "Sync", false),
        std::make_unique<juce::AudioParameterFloat> (
            "xoverFreq", "Crossover", LogRange<float> (50.0f, 20000.0f, 0.01f), 1000.0f, "Hz"),
        std::make_unique<juce::AudioParameterFloat> (
            "xoverReson", "Resonance", LogRange<float> (0.125f, 4.0f, 0.001f), 1.0f / juce::MathConstants<float>::sqrt2),
        std::make_unique<juce::AudioParameterFloat> (
            "xoverBalance", "Balance", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 0.5f),
        std::make_unique<juce::AudioParameterFloat> (
            "xoverMix", "Harmonic", juce::NormalisableRange<float> (0.0f, 1.0f, 0.001f), 1.0f)
    };
}

void HyperTremoloPlugin::initializeListeners()
{
    jassert (listeners_ids.size() == 0);
    jassert (listeners.size() == 0);

    listeners_ids.push_back ("gain");
    listeners.push_back (new SetterListener (std::bind (
        &juce::dsp::Gain<float>::setGainDecibels, &gain, std::placeholders::_1)));

    listeners_ids.push_back ("mix");
    listeners.push_back (new SetterListener (std::bind (
        &juce::dsp::DryWetMixer<float>::setWetMixProportion, &dryWet, std::placeholders::_1)));

    listeners_ids.push_back ("tremRate");
    listeners_ids.push_back ("tremZero");
    auto* t0AndF = new ThroughZeroAndFrequencySetterListener (
        std::bind (
            &DualTremolo<float>::setTremoloRate, &processor, std::placeholders::_1),
        std::bind (
            &DualTremolo<float>::setTremoloThroughZero, &processor, std::placeholders::_1),
        std::bind (
            &juce::AudioProcessorValueTreeState::getRawParameterValue, &valueTreeState, std::placeholders::_1),
        "tremRate",
        "tremZero");
    listeners.push_back (t0AndF);
    listeners.push_back (t0AndF);

    listeners_ids.push_back ("tremSync");
    listeners.push_back (new TremSyncSetterListener (std::bind (
        &DualTremolo<float>::sync, &processor)));

    listeners_ids.push_back ("tremRatio");
    listeners.push_back (new SetterListener (std::bind (
        &DualTremolo<float>::setTremoloRatio, &processor, std::placeholders::_1)));

    listeners_ids.push_back ("tremMix");
    listeners.push_back (new SetterListener (std::bind (
        &DualTremolo<float>::setTremoloMix, &processor, std::placeholders::_1)));

    listeners_ids.push_back ("xoverFreq");
    listeners.push_back (new SetterListener (std::bind (
        &DualTremolo<float>::setCrossoverFrequency, &processor, std::placeholders::_1)));

    listeners_ids.push_back ("xoverReson");
    listeners.push_back (new SetterListener (std::bind (
        &DualTremolo<float>::setCrossoverResonance, &processor, std::placeholders::_1)));

    listeners_ids.push_back ("xoverBalance");
    listeners.push_back (new SetterListener (std::bind (
        &DualTremolo<float>::setCrossoverBalance, &processor, std::placeholders::_1)));

    listeners_ids.push_back ("xoverMix");
    listeners.push_back (new SetterListener (std::bind (
        &DualTremolo<float>::setCrossoverMix, &processor, std::placeholders::_1)));

    jassert (listeners_ids.size() == listeners.size());

    auto n = listeners.size();
    for (unsigned int i = 0; i < n; ++i)
        valueTreeState.addParameterListener (listeners_ids[i], listeners[i]);
}
