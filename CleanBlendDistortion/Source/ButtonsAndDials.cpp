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
    addTransferObjWithLabel(&mFuzzVoltageTransferObj, &mFuzzVoltageTransferObjLabel, "Fuzz");
    
    // Distortion Graphic
    addTransferObjWithLabel(&mDistortionVoltageTransferObj, &mDistortionVoltageTransferObjLabel, "Distortion");
    
    // FWR Graphic
    addTransferObjWithLabel(&mFWRVoltageTransferObj, &mFWRVoltageTransferObjLabel, "FWR");
}

ButtonsAndDials::~ButtonsAndDials()
{
}

void ButtonsAndDials::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);   // clear the background
    g.setColour(juce::Colours::white);
    
    auto FuzzColour = mFuzzEffectToggle.getToggleState() ? juce::Colours::white : juce::Colours::grey;
    mFuzzVoltageTransferObjLabel.setColour(juce::Label::textColourId, FuzzColour);
    mFuzzGainSlider.setColour(juce::Slider::ColourIds::thumbColourId, FuzzColour);
    mFuzzGainLabel.setColour(juce::Label::textColourId, FuzzColour);
    mFuzzVolumeSlider.setColour(juce::Slider::ColourIds::thumbColourId, FuzzColour);
    mFuzzVolumeLabel.setColour(juce::Label::textColourId, FuzzColour);
    
    auto DistortionColour = mDistortionEffectToggle.getToggleState() ? juce::Colours::white : juce::Colours::grey;
    mDistortionVoltageTransferObjLabel.setColour(juce::Label::textColourId, DistortionColour);
    mDistortionGainSlider.setColour(juce::Slider::ColourIds::thumbColourId, DistortionColour);
    mDistortionGainLabel.setColour(juce::Label::textColourId, DistortionColour);
    mDistortionVolumeSlider.setColour(juce::Slider::ColourIds::thumbColourId, DistortionColour);
    mDistortionVolumeLabel.setColour(juce::Label::textColourId, DistortionColour);
    
    auto FullWaveRectifierColour = mFullWaveRectifierToggle.getToggleState() ? juce::Colours::white : juce::Colours::grey;
    mFWRVoltageTransferObjLabel.setColour(juce::Label::textColourId, FullWaveRectifierColour);
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
    mFuzzEffectToggle.setBoundsRelative(startX+n*dialWidth, startY+0.58f, dialWidth*0.3, 0.2);n+=1;
    mFuzzGainSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight);n+=1;
    mFuzzVolumeSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth/2.0f, dialHeight);n+=0.5;
    
    // Distortion
    mDistortionVoltageTransferObj.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight*0.8);
    mDistortionEffectToggle.setBoundsRelative(startX+n*dialWidth, startY+0.58f, dialWidth*0.3, 0.2);n+=1;
    mDistortionGainSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight);n++;
    mDistortionVolumeSlider.setBoundsRelative(startX+n*dialWidth, startY, dialWidth/2.0f, dialHeight);n+=0.5;
    
    // Full wave rectifier
    mFWRVoltageTransferObj.setBoundsRelative(startX+n*dialWidth, startY, dialWidth, dialHeight*0.8);
    mFullWaveRectifierToggle.setBoundsRelative(startX+n*dialWidth, startY+0.58f, dialWidth*0.5, 0.2);n+=1;
    
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

void ButtonsAndDials::addTransferObjWithLabel(ViewVoltageTransfer* trans, juce::Label* labelObj, std::string label_text)
{
    addAndMakeVisible(trans);
    // Add label
    labelObj->setFont(15.f);
    labelObj->setText(label_text, juce::NotificationType::dontSendNotification);
    labelObj->setJustificationType(juce::Justification::horizontallyCentred);
    labelObj->attachToComponent(trans, false);
    addAndMakeVisible(labelObj);
}

