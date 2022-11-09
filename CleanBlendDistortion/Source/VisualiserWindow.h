/*
  ==============================================================================

    VisualiserWindow.h
    Created: 9 Nov 2022 7:41:36pm
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class VisualiserWindow  : public juce::Component, public juce::Timer
{
public:
    VisualiserWindow();
    ~VisualiserWindow() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    juce::Rectangle<float> outerBounds;
    juce::Rectangle<float> innerBounds;
    juce::Path generateRandomPath(juce::Rectangle<float> Rect);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VisualiserWindow)
};
