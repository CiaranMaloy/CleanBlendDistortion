/*
  ==============================================================================

    VisualiserWindow.h
    Created: 9 Nov 2022 7:41:36pm
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CircularBuffer.h"

//==============================================================================
/*
*/
class VisualiserWindow : public juce::Component, public juce::Timer
{
public:
    VisualiserWindow(CleanBlendDistortionAudioProcessor& p);
    ~VisualiserWindow() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    juce::Rectangle<float> outerBounds;
    juce::Rectangle<float> innerBounds;
    juce::Path generateRandomPath(juce::Rectangle<float> Rect);
    juce::Path generateAudioPath(juce::Rectangle<float> Rect, const int CHANNEL);
    
    CleanBlendDistortionAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualiserWindow)
};
