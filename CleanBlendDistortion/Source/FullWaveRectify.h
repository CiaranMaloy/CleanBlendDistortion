/*
  ==============================================================================

    FullWaveRectify.h
    Created: 4 Nov 2022 8:32:57pm
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FullWaveRectifyEffect
// From HERE: to initialise with: (juceAudioBuffer<float>& buffer, int totalNumInputChannels)
// constructor does nothing really and it's kept as a private variable
// FullWaveRectify.process(juceAudioBuffer<float>& buffer, int totalNumInputChannels);
{
public:
    static void process(juce::AudioBuffer<float>& buffer, int totalNumInputChannels);
};
