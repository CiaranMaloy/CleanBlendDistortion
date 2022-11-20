/*
  ==============================================================================

    WaveShaping.cpp
    Created: 20 Nov 2022 7:49:15am
    Author:  ciaran maloy

  ==============================================================================
*/

#include "WaveShaping.h"

void WaveShaping::process(EffectType type, juce::AudioBuffer<float> &buffer, int totalNumInputChannels, float gain)
{
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        for (int channel = 0; channel < totalNumInputChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer (channel);
            
            switch (type)
            {
                case EffectType::fuzz:
                    channelData[sample] = fexp1(channelData[sample], gain);
                    break;
                    
                case EffectType::distortion:
                    channelData[sample] = asqrt_sqs(channelData[sample]);
                    break;
            }
        }
    }
}

// Fuzz
float WaveShaping::fexp1(float sampleValue, float gain=1.0)
{
    return sgn(sampleValue)*((1.0f-exp(-abs(sampleValue)))/((1.0f-exp(-abs(gain)))));
}

// Distortion
float WaveShaping::asqrt(float sampleValue)
{
    return sqrt(abs(sampleValue));
}

float WaveShaping::sqs(float sampleValue)
{
    return pow(sampleValue, 2.0f)*sgn(sampleValue);
}

// Asymetrical
float WaveShaping::asqrt_sqs(float sampleValue)
{
    return (sampleValue > 0) ? asqrt(sampleValue) : sqs(sampleValue);
}

// Maths
float WaveShaping::sgn(float x)
{
    return (x > 0.0) - (x < 0.0);
}
