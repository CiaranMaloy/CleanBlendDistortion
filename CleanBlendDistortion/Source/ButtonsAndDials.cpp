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
ButtonsAndDials::ButtonsAndDials(CleanBlendDistortionAudioProcessor& p) : mDistortionEffectToggle(), mFullWaveRectifierToggle(), mFuzzGainSlider(), mDryFilterFreqSlider(), mDryFilterResSlider(), mWetDryMixRatioSlider(), mDistortionEffectToggleLabel(), mFullWaveRectifierToggleLabel(), mFuzzGainLabel(), mDryFilterFreqLabel(), mDryFilterResLabel(), mWetDryMixRatioLabel(), audioProcessor(p)
{
    // Add toggle
    addToggleWithLabel(&mDistortionEffectToggle, &mDistortionEffectToggleLabel, "Distortion");
    addToggleWithLabel(&mFullWaveRectifierToggle, &mFullWaveRectifierToggleLabel, "Full Wave Rect");
    
    // Add sliders and labels
    addSliderWithLabel(&mFuzzGainSlider, &mFuzzGainLabel, "Fuzz Gain");
    addSliderWithLabel(&mDistortionGainSlider, &mDistortionGainLabel, "Distortion Gain");
    addSliderWithLabel(&mDryFilterFreqSlider, &mDryFilterFreqLabel, "Dry Filter Freq", 500.0);
    addSliderWithLabel(&mDryFilterResSlider, &mDryFilterResLabel, "Dry Filter Res");
    addSliderWithLabel(&mWetDryMixRatioSlider, &mWetDryMixRatioLabel, "Wet/Dry");
    
    // attach to Audio Processor Value Tree State
    // Buttons
    mDistortionEffectToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "DISTORTION TOGGLE", mDistortionEffectToggle);
    
    mFullWaveRectifierToggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getAPVTS(), "FULL WAVE RECTIFIER TOGGLE", mFullWaveRectifierToggle);
    
    
    // Sliders
    mFuzzGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "FUZZ GAIN", mFuzzGainSlider);
    
    mDistortionGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DISTORTION GAIN", mDistortionGainSlider);
    
    mDryFilterFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DRY FILTER FREQ", mDryFilterFreqSlider);
 
    mDryFilterResAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "DRY FILTER RES", mDryFilterResSlider);
    
    mWetDryMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getAPVTS(), "WET/DRY", mWetDryMixRatioSlider);
    
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
    const auto dialWidth = 0.15f;
    const auto dialHeight = 0.75f;
    
    mFuzzGainSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDistortionGainSlider.setBoundsRelative(startX+dialWidth, startY, dialWidth, dialHeight);
    mDistortionEffectToggle.setBoundsRelative(startX+2.0f*dialWidth, startY, dialWidth, dialHeight/2.0f);
    mFullWaveRectifierToggle.setBoundsRelative(startX+2.0f*dialWidth, 3.0f*startY, dialWidth, dialHeight/2.0f);
    mDryFilterFreqSlider.setBoundsRelative(startX+3.0f*dialWidth, startY, dialWidth, dialHeight);
    mDryFilterResSlider.setBoundsRelative(startX+4.0f*dialWidth, startY, dialWidth, dialHeight);
    mWetDryMixRatioSlider.setBoundsRelative(startX+5.0f*dialWidth, startY, dialWidth, dialHeight);
}

void ButtonsAndDials::addSliderWithLabel(juce::Slider* sliderObj, juce::Label* labelObj, std::string label_text, double centre_point)
{
    // Create Sliders
    sliderObj->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderObj->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 20);
    sliderObj->setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    
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

