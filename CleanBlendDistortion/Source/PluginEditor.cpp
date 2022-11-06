/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CleanBlendDistortionAudioProcessorEditor::CleanBlendDistortionAudioProcessorEditor (CleanBlendDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), ButtonsAndDialsObj(), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(ButtonsAndDialsObj);
    setSize (400, 300);
}

CleanBlendDistortionAudioProcessorEditor::~CleanBlendDistortionAudioProcessorEditor()
{
}

//==============================================================================
void CleanBlendDistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::green);
    g.setColour(juce::Colours::white);
    
}

void CleanBlendDistortionAudioProcessorEditor::resized()
{
    const float bnd_x_pos_relative = 0;
    const float bnd_y_pos_relative = 0.5;
    
    ButtonsAndDialsObj.setBoundsRelative(bnd_x_pos_relative, bnd_y_pos_relative, 1.0f, 0.25f);
}

