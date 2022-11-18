/*
  ==============================================================================

    Fuzz.h
    Created: 18 Nov 2022 12:22:22am
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <math.h>

class FuzzEffect
{
public:
    static void process(juce::AudioBuffer<float>& buffer, int totalNumInputChannels, float gain);
    
private:
    static float sgn(float x);
};
