/*
  ==============================================================================

    ViewVoltageTransfer.h
    Created: 20 Nov 2022 10:10:23am
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WaveShaping.h"

//==============================================================================
/*
*/
class ViewVoltageTransfer  : public juce::Component
{
public:
    ViewVoltageTransfer();
    ~ViewVoltageTransfer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // == rects
    juce::Rectangle<float> outerBounds;
    juce::Rectangle<float> innerBounds;
    
    // == voltage transfer
    juce::Path generateVoltageTransferPath(juce::Rectangle<float> Rect);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViewVoltageTransfer)
};
