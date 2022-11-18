/*
  ==============================================================================

    SquareSign.cpp
    Created: 18 Nov 2022 1:04:31am
    Author:  ciaran maloy

  ==============================================================================
*/

#include "SquareSign.h"

//void SquareSignEffect::process(juce::AudioBuffer<float> &buffer, int totalNumInputChannels)
//{
//    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
//    {
//        for (int channel = 0; channel < totalNumInputChannels; ++channel)
//        {
//            float* channelData = buffer.getWritePointer (channel);
//
//            channelData[sample] = pow(channelData[sample], 2.0f)*sgn(channelData[sample]);
//        }
//    }
//}

void SquareSignEffect::process(juce::AudioBuffer<float> &buffer, int totalNumInputChannels)
{
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer (channel);

            channelData[sample] = (channelData[sample] > 0) ? sqrt(abs(channelData[sample])) : pow(channelData[sample], 2.0f)*sgn(channelData[sample]);
        }
    }
}

float SquareSignEffect::sgn(float x)
{
    return (x > 0.0) - (x < 0.0);
}
