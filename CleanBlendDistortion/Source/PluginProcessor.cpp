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
                       ), mLowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 20000.0f, 0.1f)), apvts(*this, nullptr, "PROCESSOR", createAPVTSParameterLayout()), mCircBuffer()
#endif
{
}

CleanBlendDistortionAudioProcessor::~CleanBlendDistortionAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout CleanBlendDistortionAudioProcessor::createAPVTSParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FUZZ GAIN", "Fuzz Gain", 0.0f, 4.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DISTORTION GAIN", "Distortion Gain", 0.0f, 4.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRY FILTER FREQ", "Dry Filter Freq", 20.0f, 20000.0f, 500.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRY FILTER RES", "Dry Filter Res", 0.0f, 2.0f, 0.707f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WET/DRY", "Wet/Dry", 0.0f, 1.0f, 0.5f));
    
    return {params.begin(), params.end()};
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
    mLastSamplerate = sampleRate;
    
    // ====== DSP Stuff ======
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    mLowPassFilter.prepare(spec);
    mLowPassFilter.reset();
    // ============
    
    // ====== Circular Buffer ======
    const int numSeconds = 5;
    mCircBuffer.setConstants(numSeconds, getTotalNumInputChannels(), sampleRate, samplesPerBlock);
    // ============
    
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

void CleanBlendDistortionAudioProcessor::updateFilter()
{
    float freq = apvts.getRawParameterValue("DRY FILTER FREQ")->load();
    float res = apvts.getRawParameterValue("DRY FILTER RES")->load();
    *mLowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(mLastSamplerate, freq, res);
}

void CleanBlendDistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // TODO: Pull this buffer using somethign threadsafe
    // TODO: Downsampling should also be done here so so that less info needs to be copied out on the thread
    // ================ EFFECTS =======================================================================
    
    
    // === Set Values
    mFuzzGainArr[1] = apvts.getRawParameterValue("FUZZ GAIN")->load();
    mDistortionGainArr[1] = apvts.getRawParameterValue("DISTORTION GAIN")->load();
    mMixArr[1] = apvts.getRawParameterValue("WET/DRY")->load();
    
    // ================ INITIALISE DRY BUFFER ======================
    mWDMEffect.storeDryBuffer(buffer, totalNumInputChannels);
    // =============================================================
    
    // ================ GAIN STAGE 1 ===============================
    buffer.applyGainRamp(0, buffer.getNumSamples(), mFuzzGainArr[0], mFuzzGainArr[1]);
    // =============================================================
    // ================= FUZZ ======================================
    FuzzEffect::process(buffer, totalNumInputChannels, mFuzzGainArr[1]);
    // =============================================================
    // ================ GAIN STAGE 2 ===============================
    buffer.applyGainRamp(0, buffer.getNumSamples(), mDistortionGainArr[0], mDistortionGainArr[1]);
    // =============================================================
    // ================= Asymetrical ===============================
    SquareSignEffect::process(buffer, totalNumInputChannels);
    //FuzzEffect::process(buffer, totalNumInputChannels, mDistortionGainArr[1]);
    // =============================================================
    
    // ================ FULLWAVERECTIFY ============================
    //FullWaveRectifyEffect::process(buffer, totalNumInputChannels);
    // =============================================================
    
    // ================ CLIP TRIGGER ===============================
    mMaxAbsVal = buffer.getMagnitude(0, buffer.getNumSamples());
    if (mMaxAbsVal > 1.0f) { mClipping = true; }
    // =============================================================
    
    // ================ LOW PASS FILTER ON DRY SIGNAL ===============
    juce::dsp::AudioBlock<float> block(*mWDMEffect.getBufferPointer());
    updateFilter();
    mLowPassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    // ==============================================================
    
    // ================ MIX IN DRY/WET SIGNALS ======================
    mWDMEffect.mixSignals(buffer, totalNumInputChannels, mMixArr[0], mMixArr[1]);
    // =============================================================
    
    // ================ Circular Buffer ============================
    // Reduce this section to be self contained
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        const float* bufferData = buffer.getReadPointer(channel);
        mCircBuffer.fillCircularBuffer(channel, buffer.getNumSamples(), bufferData);
    }
    mCircBuffer.updateWritePosition(buffer.getNumSamples());
    // =============================================================
    
    // === Reset Values
    mFuzzGainArr[0] = mFuzzGainArr[1];
    mDistortionGainArr[0] = mDistortionGainArr[1];
    mMixArr[0] = mMixArr[1];
}

// return data from circular buffer
/* return the last n samples from behind the mWritePosition within the circular buffer */
juce::AudioBuffer<float> CleanBlendDistortionAudioProcessor::getBufferForDisplay()
{
    return mCircBuffer.getBufferForDisplay();
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
