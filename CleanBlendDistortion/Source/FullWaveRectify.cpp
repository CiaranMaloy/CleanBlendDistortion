/*
  ==============================================================================

    FullWaveRectify.cpp
    Created: 4 Nov 2022 8:32:57pm
    Author:  ciaran maloy

  ==============================================================================
*/

#include "FullWaveRectify.h"

void FullWaveRectifyEffect::process(juce::AudioBuffer<float> &buffer, int totalNumInputChannels)
{
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            auto* channelData = buffer.getWritePointer (channel);
            
            // full wave rectifier (if less than 0, return )
            channelData[sample] = (channelData[sample] > 0) ? channelData[sample] : -channelData[sample];
        }
    }
}
