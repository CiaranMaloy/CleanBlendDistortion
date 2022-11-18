/*
  ==============================================================================

    Fuzz.cpp
    Created: 18 Nov 2022 12:22:22am
    Author:  ciaran maloy

  ==============================================================================
*/

#include "Fuzz.h"


void FuzzEffect::process(juce::AudioBuffer<float> &buffer, int totalNumInputChannels, float gain)
{
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer (channel);
            
            channelData[sample] = sgn(channelData[sample])*((1.0f-exp(-abs(channelData[sample])))/((1.0f-exp(-abs(gain)))));
        }
    }
}

float FuzzEffect::sgn(float x)
{
    return (x > 0.0) - (x < 0.0);
}
