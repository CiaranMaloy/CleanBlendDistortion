/*
  ==============================================================================

    SquareSign.h
    Created: 18 Nov 2022 1:04:31am
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "math.h"


class SquareSignEffect
{
public:
    static void process(juce::AudioBuffer<float>& buffer, int totalNumInputChannels);
    
private:
    static float sgn(float x);
};
