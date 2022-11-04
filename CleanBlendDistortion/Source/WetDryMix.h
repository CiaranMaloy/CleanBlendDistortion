/*
  ==============================================================================

    WetDryMix.h
    Created: 4 Nov 2022 7:34:32pm
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class WetDryMixEffect
{
    void storeDryBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels);
    /* storeDryBuffer will make the member variable mDryBuffer a copy of the input buffer*/
    
    void mixSignals(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, float mixAmount);
    /* mixSignals takes the pointer to buffer object and mixes that signal with the mDryBuffer object*/
    
private:
    juce::AudioBuffer<float>& mDryBuffer;
};
