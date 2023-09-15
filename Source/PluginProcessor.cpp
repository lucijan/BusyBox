/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr double kSimulatedLoad = 0.6;

//==============================================================================
BusyBoxAudioProcessor::BusyBoxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    oscillator_.initialise([](float p){
        return std::sin(p);
    });

    oscillator_.setFrequency(350.0);
}

BusyBoxAudioProcessor::~BusyBoxAudioProcessor()
{
}

//==============================================================================
const juce::String BusyBoxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BusyBoxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BusyBoxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BusyBoxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BusyBoxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BusyBoxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BusyBoxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BusyBoxAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BusyBoxAudioProcessor::getProgramName (int index)
{
    return {};
}

void BusyBoxAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BusyBoxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;

    oscillator_.prepare(spec);
    loadMeasurer_.reset(sampleRate, samplesPerBlock);
}

void BusyBoxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BusyBoxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BusyBoxAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    double begin = juce::Time::getMillisecondCounterHiRes();
    double max = kSimulatedLoad * (buffer.getNumSamples() / getSampleRate());
    juce::AudioProcessLoadMeasurer::ScopedTimer timer(loadMeasurer_);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    oscillator_.process(context);
    block.multiplyBy(0.8f);

    volatile int counter = 0;
    while ((juce::Time::getMillisecondCounterHiRes() - begin) / 1000.0 < max) {
        counter++;
    }
}

//==============================================================================
bool BusyBoxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BusyBoxAudioProcessor::createEditor()
{
    return new BusyBoxAudioProcessorEditor (*this);
}

//==============================================================================
void BusyBoxAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BusyBoxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BusyBoxAudioProcessor();
}
