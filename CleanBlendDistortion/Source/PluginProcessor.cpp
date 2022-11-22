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
                       ), effectParams(), mLowPassFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(), 20000.0f, 1.0f)), apvts(*this, nullptr, "PROCESSOR", createAPVTSParameterLayout()), mCircBuffer()
#endif
{
}

CleanBlendDistortionAudioProcessor::~CleanBlendDistortionAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout CleanBlendDistortionAudioProcessor::createAPVTSParameterLayout()
{
    float LPF_MIN = 20.0f;
    float LPF_MAX = 20000.0f;
    float LPF_INITIAL_VALUE = 500.0f;
    float LPF_MID_POINT = 1000.0f;
    float LPF_SKEW_FACTOR = log(0.5f) / log((LPF_MID_POINT-LPF_MIN)/(LPF_MAX-LPF_MIN));
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Bools
    params.push_back(std::make_unique<juce::AudioParameterBool>("DISTORTION TOGGLE", "Distortion Toggle", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("FULL WAVE RECTIFIER TOGGLE", "Full Wave Rectifier Toggle", true));
    
    // Floats
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FUZZ GAIN", "Fuzz Gain", 0.0f, 4.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DISTORTION GAIN", "Distortion Gain", 0.0f, 4.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRY FILTER FREQ", "Dry Filter Freq", juce::NormalisableRange<float>(LPF_MIN, LPF_MAX, 1.f, LPF_SKEW_FACTOR), LPF_INITIAL_VALUE));
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
    mCircBuffer.setConstants(numSeconds, getTotalNumInputChannels()*2, sampleRate, samplesPerBlock);
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
    
    // ================ Circular Buffer ============================
    for (int channel = 0; channel < totalNumInputChannels; channel++)
    {
        const float* bufferData = buffer.getReadPointer(channel);
        mCircBuffer.fillCircularBuffer(channel, buffer.getNumSamples(), bufferData);
    }
    
    // ================ EFFECTS =======================================================================
    // === Set Values
    updateEffectParameters();
    
    // ================ INITIALISE DRY BUFFER ======================
    mWDMEffect.storeDryBuffer(buffer, totalNumInputChannels);
    // =============================================================
    
    // ================= FUZZ ======================================
    // Input Gain
    buffer.applyGainRamp(0, buffer.getNumSamples(), effectParams.mFuzzGainArr[0], effectParams.mFuzzGainArr[1]);
    // Fuzz
    WaveShaping::process(WaveShaping::EffectType::fuzz, buffer, totalNumInputChannels, effectParams.mFuzzGainArr[1]);
    // =============================================================
    
    // ================= DISTORTION ================================
    if (effectParams.mDistortionEffectBool)
    {
        // Input Gain
        buffer.applyGainRamp(0, buffer.getNumSamples(), effectParams.mDistortionGainArr[0], effectParams.mDistortionGainArr[1]);
        
        // Distortion
        WaveShaping::process(WaveShaping::EffectType::distortion, buffer, totalNumInputChannels);
    }
    // =============================================================
    
    // ================ FULLWAVERECTIFY ============================
    if (effectParams.mFullWaveRectifierBool)
    {
        WaveShaping::process(WaveShaping::EffectType::fullWaveRectifier, buffer, totalNumInputChannels);
    }
    // =============================================================
    
    // ================ CLIP TRIGGER ===============================
//    mMaxAbsVal = buffer.getMagnitude(0, buffer.getNumSamples());
//    if (mMaxAbsVal > 1.0f) { mClipping = true; }
    // =============================================================
    
    // ================ LOW PASS FILTER ON DRY SIGNAL ===============
    juce::dsp::AudioBlock<float> block(*mWDMEffect.getBufferPointer());
    updateFilter();
    mLowPassFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    // ==============================================================
    
    // ================ MIX IN DRY/WET SIGNALS ======================
    mWDMEffect.mixSignals(buffer, totalNumInputChannels, effectParams.mMixArr[0], effectParams.mMixArr[1]);
    // =============================================================
    
    // ================ Circular Buffer ============================
    for (int channel = 2; channel < totalNumInputChannels*2; channel++)
    {
        const float* bufferData = buffer.getReadPointer(channel-2);
        mCircBuffer.fillCircularBuffer(channel, buffer.getNumSamples(), bufferData);
    }
    mCircBuffer.updateWritePosition(buffer.getNumSamples());
    // =============================================================
}

// Update internal parameters
void CleanBlendDistortionAudioProcessor::updateEffectParameters()
{
    // Update old parameters (will crash if not pre-declaired)
    effectParams.mFuzzGainArr[0] = effectParams.mFuzzGainArr[1];
    effectParams.mDistortionGainArr[0] = effectParams.mDistortionGainArr[1];
    effectParams.mMixArr[0] = effectParams.mMixArr[1];
    
    // Update new parameters
    // TODO: This could be refactored
    effectParams.mDistortionEffectBool = apvts.getRawParameterValue("DISTORTION TOGGLE")->load() > 0.5;
    effectParams.mFullWaveRectifierBool = apvts.getRawParameterValue("FULL WAVE RECTIFIER TOGGLE")->load() > 0.5;
    effectParams.mFuzzGainArr[1] = apvts.getRawParameterValue("FUZZ GAIN")->load();
    effectParams.mDistortionGainArr[1] = apvts.getRawParameterValue("DISTORTION GAIN")->load();
    effectParams.mMixArr[1] = apvts.getRawParameterValue("WET/DRY")->load();
    
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
