/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WetDryMix.h"
#include "WaveShaping.h"
#include "CircularBuffer.h"

//==============================================================================
/**
*/
class CleanBlendDistortionAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    CleanBlendDistortionAudioProcessor();
    ~CleanBlendDistortionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // ====== Filters and DSP ======
    void updateFilter();
    
    // ====== APVTS =====
    juce::AudioProcessorValueTreeState& getAPVTS() {return apvts;}
    
    // ====== GUI Handover ======
    void fillCircularBuffer(juce::AudioBuffer<float>* Buffer, int channel, const int bufferLength, const int circBufferLength, const float* bufferData, const float* circBufferData);

    juce::AudioBuffer<float> getBufferForDisplay();

private:
    // ====== Audio Effect Parameters Structure ======
    struct EffectParameters
    {
        // ====== ON/OFF ======
        bool mFuzzEffectBool, mDistortionEffectBool, mFullWaveRectifierBool;
        
        // ====== GAIN ======
        std::array<float, 2> mFuzzGainArr = {1.0, 1.0};
        std::array<float, 2> mFuzzVolumeArr = {1.0, 1.0};
        std::array<float, 2> mDistortionGainArr = {1.0, 1.0};
        std::array<float, 2> mDistortionVolumeArr = {1.0, 1.0};
        std::array<float, 2> mMixArr = {0.5, 0.5};
        
        float mGainStageOne {1.2};
        float mMix {0.5};
    };
    EffectParameters effectParams;
    void updateEffectParameters();
    
    // ====== AUDIO EFFECTS ======
    WetDryMixEffect mWDMEffect;
    
    // ====== SAMPLERATE =====
    double mLastSamplerate;
    
    // ====== DSP ======
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> mLowPassFilter;
    
    // ====== APVTS ======
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createAPVTSParameterLayout();
    
    // ====== GUI Handover ======
    CircularBuffer mCircBuffer;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CleanBlendDistortionAudioProcessor)
};
