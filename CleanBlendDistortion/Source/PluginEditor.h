/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ButtonsAndDials.h"
#include "VisualiserWindow.h"
#include "ViewVoltageTransfer.h"
#include "WaveShaping.h"

//==============================================================================
/**
*/
class CleanBlendDistortionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CleanBlendDistortionAudioProcessorEditor (CleanBlendDistortionAudioProcessor&);
    ~CleanBlendDistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // ====== Buttons and Dials ======
    ButtonsAndDials ButtonsAndDialsObj;
    ViewVoltageTransfer FuzzVoltageTransferObj;
    //VisualiserWindow VisualiserWindowObj;
    // ====== ======
    
    CleanBlendDistortionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CleanBlendDistortionAudioProcessorEditor)
};
