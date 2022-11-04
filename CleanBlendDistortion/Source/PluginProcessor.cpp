/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CleanBlendDistortionAudioProcessor::CleanBlendDistortionAudioProcessor()
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
}

CleanBlendDistortionAudioProcessor::~CleanBlendDistortionAudioProcessor()
{
}

//==============================================================================
const juce::String CleanBlendDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CleanBlendDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CleanBlendDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CleanBlendDistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CleanBlendDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CleanBlendDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CleanBlendDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CleanBlendDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CleanBlendDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void CleanBlendDistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CleanBlendDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void CleanBlendDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CleanBlendDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void CleanBlendDistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    // ================ EFFECTS ====================================
    
    // ================ INITIALISE DRY BUFFER ======================
    // Duplicate the buffer object by de-referencing it (idk if theres a better way of doing this)
    /* Initialise using buffer object and number of samples to make a copy of the buffer */
    mWDMEffect.storeDryBuffer(buffer, totalNumInputChannels);
    // =============================================================
    
    // ================ FULLWAVERECTIFY ============================
    // From HERE: to initialise with: (juceAudioBuffer<float>& buffer, int totalNumInputChannels)
    // constructor does nothing really and it's kept as a private variable
    // FullWaveRectify.process(juceAudioBuffer<float>& buffer, int totalNumInputChannels);
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);
            
            // full wave rectifier (if less than 0, return )
            channelData[sample] = (channelData[sample] > 0) ? channelData[sample] : -channelData[sample];
        }
    }
    // =============================================================
    
    // ================ MIX IN DRY/WET SIGNALS ======================
    /* ISSUES:
     1. there is no assertion or check that the buffer and the dryBuffer have in them the same number of
     samples */
    /* activate by using buffer */
    mWDMEffect.mixSignals(buffer, totalNumInputChannels, mMix);
    // =============================================================
}

//==============================================================================
bool CleanBlendDistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CleanBlendDistortionAudioProcessor::createEditor()
{
    return new CleanBlendDistortionAudioProcessorEditor (*this);
}

//==============================================================================
void CleanBlendDistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CleanBlendDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CleanBlendDistortionAudioProcessor();
}
