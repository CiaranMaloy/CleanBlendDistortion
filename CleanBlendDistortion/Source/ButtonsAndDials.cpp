/*
  ==============================================================================

    ButtonsAndDials.cpp
    Created: 5 Nov 2022 8:21:18am
    Author:  ciaran maloy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ButtonsAndDials.h"

//==============================================================================
ButtonsAndDials::ButtonsAndDials(CleanBlendDistortionAudioProcessor& p) : audioProcessor(p), mFuzzVoltageTransferObj(WaveShaping::EffectType::fuzz), mDistortionVoltageTransferObj(WaveShaping::EffectType::distortion), mFWRVoltageTransferObj(WaveShaping::EffectType::fullWaveRectifier)
{
    // Add toggle
    addToggleWithLabel(&mFuzzEffectToggle, &mFuzzEffectToggleLabel, "On/Off");
    addToggleWithLabel(&mDistortionEffectToggle, &mDistortionEffectToggleLabel, "On/Off");
    addToggleWithLabel(&mFullWaveRectifierToggle, &mFullWaveRectifierToggleLabel, "On/Off");
    
    // Add sliders and labels
    addSliderWithLabel(juce::Slider::SliderStyle::RotaryVerticalDrag, &mFuzzGainSlider, &mFuzzGainLabel, "Gain", WetDryChain::wet);
    addSliderWithLabel(juce::Slider::SliderStyle::LinearVertical, &mFuzzVolumeSlider, &mFuzzVolumeLabel, "Trim", WetDryChain::wet);
    
    addSliderWithLabel(juce::Slider::SliderStyle::RotaryVerticalDrag, &mDistortionGainSlider, &mDistortionGainLabel, "Gain", WetDryChain::wet);
    addSliderWithLabel(juce::Slider::SliderStyle::LinearVertical, &mDistortionVolumeSlider, &mDistortionVolumeLabel, "Trim", WetDryChain::wet);
    
    addSliderWithLabel(juce::Slider::SliderStyle::RotaryVerticalDrag, &mDryFilterFreqSlider, &mDryFilterFreqLabel, "Dry Filter Freq", WetDryChain::wet, 500.0f);
    
    addSliderWithLabel(juce::Slider::SliderStyle::RotaryVerticalDrag, &mDryFilterResSlider, &mDryFilterResLabel, "Dry Filter Res", WetDryChain::wet);
    
    addSliderWithLabel(juce::Slider::SliderStyle::RotaryVerticalDrag, &mWetDryMixRatioSlider, &mWetDryMixRatioLabel, "Wet/Dry", WetDryChain::wet);
    
    // attach to Audio Processor Value Tree State
    // Buttons
    mFuzzEffectToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "FUZZ TOGGLE", mFuzzEffectToggle);
    
    mDistortionEffectToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "DISTORTION TOGGLE", mDistortionEffectToggle);
    
    mFullWaveRectifierToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "FULL WAVE RECTIFIER TOGGLE", mFullWaveRectifierToggle);
    
    
    // Sliders
    mFuzzGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "FUZZ GAIN", mFuzzGainSlider);
    mFuzzVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "FUZZ VOLUME", mFuzzVolumeSlider);
    
    mDistortionGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DISTORTION GAIN", mDistortionGainSlider);
    mDistortionVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DISTORTION VOLUME", mDistortionVolumeSlider);
    
    mDryFilterFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DRY FILTER FREQ", mDryFilterFreqSlider);
 
    mDryFilterResAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DRY FILTER RES", mDryFilterResSlider);
    
    mWetDryMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "WET/DRY", mWetDryMixRatioSlider);
    
    // Graphics
    // Fuzz Graphic
    addAndMakeVisible(mFuzzVoltageTransferObj);
    // Add label
    mFuzzVoltageTransferObjLabel.setFont(15.f);
    mFuzzVoltageTransferObjLabel.setText("Fuzz", juce::NotificationType::dontSendNotification);
    mFuzzVoltageTransferObjLabel.setJustificationType(juce::Justification::horizontallyCentred);
    mFuzzVoltageTransferObjLabel.attachToComponent(&mFuzzVoltageTransferObj, false);
    addAndMakeVisible(mFuzzVoltageTransferObjLabel);
    
    // Distortion Graphic
    addAndMakeVisible(mDistortionVoltageTransferObj);
    // Add label
    mDistortionVoltageTransferObjLabel.setFont(15.f);
    mDistortionVoltageTransferObjLabel.setText("Distortion", juce::NotificationType::dontSendNotification);
    mDistortionVoltageTransferObjLabel.setJustificationType(juce::Justification::horizontallyCentred);
    mDistortionVoltageTransferObjLabel.attachToComponent(&mDistortionVoltageTransferObj, false);
    addAndMakeVisible(mDistortionVoltageTransferObjLabel);
    
    // FWR Graphic
    addAndMakeVisible(mFWRVoltageTransferObj);
    // Add label
    mFWRVoltageTransferObjLabel.setFont(15.f);
    mFWRVoltageTransferObjLabel.setText("Full Wave Rectifier", juce::NotificationType::dontSendNotification);
    mFWRVoltageTransferObjLabel.setJustificationType(juce::Justification::horizontallyCentred);
    mFWRVoltageTransferObjLabel.attachToComponent(&mFWRVoltageTransferObj, false);
    addAndMakeVisible(mFWRVoltageTransferObjLabel);
}

ButtonsAndDials::~ButtonsAndDials()
{
}

void ButtonsAndDials::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);   // clear the background
    g.setColour(juce::Colours::white);
}

void ButtonsAndDials::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    const auto startX = 0.f;
    const auto startY = 0.2f;
    const auto dialWidth = 1.0f/9.0f;
    const auto dialHeight = 0.75f;
    
    float n = 0;
    
    // Fuzz
    mFuzzVoltageTransferObj.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight*0.8);
    mFuzzEffectToggle.setBoundsRelative(startX+n*dialWidth, startY+0.58f, dialWidth, 0.2);n+=1;
    mFuzzGainSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight);n+=1;
    mFuzzVolumeSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth/2.0f, dialHeight);n+=0.5;
    
    // Distortion
    mDistortionVoltageTransferObj.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight*0.8);
    mDistortionEffectToggle.setBoundsRelative(startX+n*dialWidth, startY+0.58f, dialWidth, 0.2);n+=1;
    mDistortionGainSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight);n++;
    mDistortionVolumeSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth/2.0f, dialHeight);n+=0.5;
    
    // Toggles
    mFWRVoltageTransferObj.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight*0.8);
    mFullWaveRectifierToggle.setBoundsRelative(startX+n*dialWidth, startY+0.58f, dialWidth, 0.2);n+=1;
    
    // Dry mixing
    mDryFilterFreqSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight);n++;
    mDryFilterResSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight);n++;
    mWetDryMixRatioSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight);
}

void ButtonsAndDials::addSliderWithLabel(juce::Slider::SliderStyle style, juce::Slider* sliderObj, juce::Label* labelObj, std::string label_text,  WetDryChain selection, double centre_point)
{
    // Create Sliders
    sliderObj->setSliderStyle(style);
    sliderObj->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    
    switch (selection)
    {
        case WetDryChain::wet:
            sliderObj->setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::white);
            break;
            
        case WetDryChain::dry:
            sliderObj->setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::powderblue);
            break;
        default:
            break;
    }
    
    addAndMakeVisible(sliderObj);
    
    // Add label
    labelObj->setFont(15.f);
    labelObj->setText(label_text, juce::NotificationType::dontSendNotification);
    labelObj->setJustificationType(juce::Justification::horizontallyCentred);
    labelObj->attachToComponent(sliderObj, false); // when this is true for some reason the labels show up??
    
    addAndMakeVisible(labelObj);
}

void ButtonsAndDials::addToggleWithLabel(juce::ToggleButton* toggleObj, juce::Label* labelObj, std::string label_text, double centre_point)
{
    addAndMakeVisible(toggleObj);
    
    // Add label
    labelObj->setFont(10.f);
    labelObj->setText(label_text, juce::NotificationType::dontSendNotification);
    labelObj->setJustificationType(juce::Justification::centredRight);
    labelObj->attachToComponent(toggleObj, false);
    
    addAndMakeVisible(labelObj);
}

