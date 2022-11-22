/*
  ==============================================================================

    WaveShaping.h
    Created: 20 Nov 2022 7:49:15am
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <math.h>

class WaveShaping
{
public:
    WaveShaping();
    
    enum EffectType
    {
        fuzz,
        distortion,
        fullWaveRectifier
    };
    
    static void process(EffectType type, juce::AudioBuffer<float>& buffer, int totalNumInputChannels, float gain=1.0f);
    
    juce::AudioBuffer<float> voltageTransferFunction(EffectType type, int N);
    
private:
    // Fuzz
    static float fexp1(float sampleValue, float gain);
    
    // Distortion
    static float asqrt(float sampleValue);
    static float sqs(float sampleValue);
    
    // Asymetrical
    static float asqrt_sqs(float sampleValue);
    static float asqrt_fexp1(float sampleValue, float gain);
    static float fwr(float sampleValue);
    
    // Maths
    static float sgn(float x);
    
    // Transfer function output
    juce::AudioBuffer<float> mDisplayBuffer;
};
