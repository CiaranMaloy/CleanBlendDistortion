/*
  ==============================================================================

    ButtonsAndDials.h
    Created: 5 Nov 2022 8:21:18am
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ViewVoltageTransfer.h"
#include "WaveShaping.h"

//==============================================================================
/*
 1. I'll have to add the audio processor in here later, when I use a value tree to pass over th values
 but a. just do the graphics
*/
class ButtonsAndDials  : public juce::Component
{
public:
    ButtonsAndDials(CleanBlendDistortionAudioProcessor& p);
    ~ButtonsAndDials() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // ====== Selection enum ======
    enum WetDryChain
    {
        wet,
        dry
    };
    
    // ====== Sliders and Labels ======
    juce::ToggleButton mDistortionEffectToggle, mFullWaveRectifierToggle;
    juce::Slider mFuzzGainSlider, mFuzzVolumeSlider, mDistortionGainSlider, mDistortionVolumeSlider, mDryFilterFreqSlider, mDryFilterResSlider, mWetDryMixRatioSlider;
    juce::Label mDistortionEffectToggleLabel, mFullWaveRectifierToggleLabel, mFuzzGainLabel, mFuzzVolumeLabel, mDistortionGainLabel, mDistortionVolumeLabel, mDryFilterFreqLabel, mDryFilterResLabel, mWetDryMixRatioLabel;
    
    
    void addSliderWithLabel(juce::Slider::SliderStyle style, juce::Slider* sliderObj, juce::Label* labelObj, std::string labelText, WetDryChain selection, double centre_point=0);
    void addToggleWithLabel(juce::ToggleButton* toggleObj, juce::Label* labelObj, std::string label_text, double centre_point=0);
    // ====== ======
    
    // ====== audio processor value tree state attachments =====
    // Buttons
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mDistortionEffectToggleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mFullWaveRectifierToggleAttachment;
    
    // Sliders
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mFuzzGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mFuzzVolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDistortionGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDistortionVolumeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mWetDryMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDryFilterFreqAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDryFilterResAttachment;
    // ====== ======
    
    // ====== Audio Processor Pointer ======
    CleanBlendDistortionAudioProcessor& audioProcessor;
    // ====== ======
    
    // ====== Visuals ======
    ViewVoltageTransfer mFuzzVoltageTransferObj;
    ViewVoltageTransfer mDistortionVoltageTransferObj;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonsAndDials)
};

