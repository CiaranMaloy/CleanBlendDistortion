/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CleanBlendDistortionAudioProcessorEditor::CleanBlendDistortionAudioProcessorEditor (CleanBlendDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), ButtonsAndDialsObj(p), VisualiserWindowObj(p), audioProcessor (p)
{
    addAndMakeVisible(ButtonsAndDialsObj);
    addAndMakeVisible(VisualiserWindowObj);
    setSize (800, 250);
}

CleanBlendDistortionAudioProcessorEditor::~CleanBlendDistortionAudioProcessorEditor()
{
}

//==============================================================================
void CleanBlendDistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::green);
    g.setColour(juce::Colours::white);
}

void CleanBlendDistortionAudioProcessorEditor::resized()
{
    const float bnd_x_pos_relative = 0;
    const float bnd_y_pos_relative = 0.4;
    
    ButtonsAndDialsObj.setBoundsRelative(bnd_x_pos_relative, bnd_y_pos_relative, 1.0f-bnd_x_pos_relative, 1.0f-bnd_y_pos_relative);
    
    const float vis_x_pos_relative = 0;
    const float vis_y_pos_relative = 0;
    const float vis_x_width_relative = 1;
    const float vis_y_height_relative = bnd_y_pos_relative;

    VisualiserWindowObj.setBoundsRelative(vis_x_pos_relative, vis_y_pos_relative, vis_x_width_relative, vis_y_height_relative);
}

