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
#include <math.h>

//==============================================================================
/*
*/
class ViewVoltageTransfer  : public juce::Component
{
public:
    ViewVoltageTransfer(WaveShaping::EffectType type);
    ~ViewVoltageTransfer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // == rects
    juce::Rectangle<float> outerBounds;
    juce::Rectangle<float> innerBounds;
    
    // == member variable buffer
//    juce::AudioBuffer<float> mDisplayBuffer;
    
    // == voltage transfer
    juce::Path generateVoltageTransferPath(juce::Rectangle<float> Rect);
    juce::Path generateVoltageTransferPath(juce::Rectangle<float> Rect, WaveShaping::EffectType type);
    
    // == Settings
    WaveShaping::EffectType mInternalType;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViewVoltageTransfer)
};
