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
ButtonsAndDials::ButtonsAndDials() : mWetGainStageOneSlider(), mWetGainStageOneLabel()
{
    addSliderWithLabel(&mWetGainStageOneSlider, &mWetGainStageOneLabel, "Wet Gain 1");
    addSliderWithLabel(&mDryFilterResSlider, &mDryFilterResLabel, "Dry Filter Res");
    addSliderWithLabel(&mDryFilterFreqSlider, &mDryFilterFreqLabel, "Dry Filter Freq");
    addSliderWithLabel(&mWetDryMixRatioSlider, &mWetDryMixRatioLabel, "Wet/Dry");
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
    const auto startY = 0.f;
    const auto dialWidth = 0.25f;
    const auto dialHeight = 1.0f;
    
    mWetGainStageOneSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDryFilterFreqSlider.setBoundsRelative(startX+dialWidth, startY, dialWidth, dialHeight);
    mDryFilterResSlider.setBoundsRelative(startX+2.0f*dialWidth, startY, dialWidth, dialHeight);
    mWetDryMixRatioSlider.setBoundsRelative(startX+3.0f*dialWidth, startY, dialWidth, dialHeight);
}

void ButtonsAndDials::addSliderWithLabel(juce::Slider* sliderObj, juce::Label* labelObj, std::string labelText)
{
    sliderObj->setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderObj->setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 10);
    sliderObj->setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    
    // Add label
    // TODO: Currently, the label isn't showing on the effect and idk why
    labelObj->setFont(10.f);
    labelObj->setText(labelText, juce::NotificationType::dontSendNotification);
    labelObj->setJustificationType(juce::Justification::horizontallyCentred);
    labelObj->attachToComponent(sliderObj, false); // when this is true for some reason the labels show up??
    
    addAndMakeVisible(sliderObj);
}

