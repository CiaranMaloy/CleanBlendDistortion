/*
  ==============================================================================

    WaveShaping.cpp
    Created: 20 Nov 2022 7:49:15am
    Author:  ciaran maloy

  ==============================================================================
*/

#include "WaveShaping.h"

// For use in audio buffer
void WaveShaping::process(EffectType type, juce::AudioBuffer<float> &buffer, int totalNumInputChannels, float gain)
{
    for (int channel = 0; channel < totalNumInputChannels; channel++)
    {
        float* channelData = buffer.getWritePointer (channel);
        for (int sample = 0; sample < buffer.getNumSamples(); sample++)
        {
            switch (type)
            {
                case EffectType::fuzz:
                    channelData[sample] = fexp1(channelData[sample], gain);
                    break;
                    
                case EffectType::distortion:
                    channelData[sample] = asqrt_sqs(channelData[sample]);
                    break;
                    
                case EffectType::fullWaveRectifier:
                    channelData[sample] = fwr(channelData[sample]);
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

float WaveShaping::fwr(float sampleValue)
{
    return (sampleValue > 0) ? sampleValue : -sampleValue;
}

// Maths
float WaveShaping::sgn(float x)
{
    return (x > 0.0) - (x < 0.0);
}


// Transfer function out
WaveShaping::WaveShaping()
{
    // Constructor
}

juce::AudioBuffer<float> WaveShaping::voltageTransferFunction(EffectType type, int N)
{
    DBG("Voltage Transfer Function");
    int CHANNEL = 0;
    int NCHANNELS = 1;
    mDisplayBuffer.setSize(NCHANNELS, N);
    
    // === writer -1 to 1 in the buffer
    for (int sample = 0; sample < mDisplayBuffer.getNumSamples(); sample++)
    {
        DBG(sample);
        float input = (static_cast<float>(sample) - static_cast<float>(N)/2.0f)/(static_cast<float>(N)/2.0f);
        DBG(input);
        mDisplayBuffer.setSample(CHANNEL, sample, input);
    }
    // ======
    // ====== Put the audio throught the effect
    DBG("Distortion Effect");
    
    switch (type)
    {
        case EffectType::distortion:
            DBG("1");
            process(WaveShaping::EffectType::distortion, mDisplayBuffer, NCHANNELS);
            process(WaveShaping::EffectType::fuzz, mDisplayBuffer, NCHANNELS);
            break;
        
        case EffectType::fuzz:
            DBG("2");
            process(WaveShaping::EffectType::fuzz, mDisplayBuffer, NCHANNELS);
            break;
            
        case EffectType::fullWaveRectifier:
            DBG("3");
            process(WaveShaping::EffectType::fullWaveRectifier, mDisplayBuffer, NCHANNELS);
            break;
    }
    
    return mDisplayBuffer;
}
