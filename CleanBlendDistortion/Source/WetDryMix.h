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
/* ISSUES:
 1. there is no assertion or check that the buffer and the dryBuffer have in them the same number of
 samples */
{
public:
    WetDryMixEffect();
    
    void storeDryBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels);
    
    void mixSignals(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, float prevMixAmount, float mixAmount);
    
    juce::AudioBuffer<float>* getBufferPointer();
private:
    juce::AudioBuffer<float> mDryBuffer;
};
