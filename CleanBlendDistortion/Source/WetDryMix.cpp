/*
  ==============================================================================

    WetDryMix.cpp
    Created: 4 Nov 2022 7:34:32pm
    Author:  ciaran maloy

  ==============================================================================
*/

#include "WetDryMix.h"

WetDryMixEffect::WetDryMixEffect()
{
    
}

void WetDryMixEffect::storeDryBuffer(juce::AudioBuffer<float>& buffer, int totalNumInputChannels)
{
    mDryBuffer.makeCopyOf(buffer);
}

void WetDryMixEffect::mixSignals(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, float prevMixAmount, float mixAmount)
{
    buffer.applyGainRamp(0, buffer.getNumSamples(), prevMixAmount, mixAmount);
    mDryBuffer.applyGainRamp(0, mDryBuffer.getNumSamples(), 1.0f-prevMixAmount, 1.0f-mixAmount);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        buffer.addFrom(channel, 0, mDryBuffer, channel, 0, mDryBuffer.getNumSamples());
    }
}

juce::AudioBuffer<float>* WetDryMixEffect::getBufferPointer()
{
    return &mDryBuffer;
}

