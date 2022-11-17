/*
  ==============================================================================

    CircularBuffer.h
    Created: 17 Nov 2022 4:54:41pm
    Author:  ciaran maloy

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CircularBuffer
{
public:
    CircularBuffer();
    
    void setConstants(const int numSeconds, const int numInputChannels, const int sampleRate, const int samplesPerBlock);
    
    // ====== Circular Buffer ======
    void fillCircularBuffer(int channel, const int bufferLength, const float* bufferData);

    void updateWritePosition(const int bufferLength);
    
    juce::AudioBuffer<float> getBuffer() {return mCircBuffer;}
    int getWritePosition() {return mWritePosition;}
    juce::AudioBuffer<float> getBufferForDisplay();
    
private:
    juce::AudioBuffer<float> mCircBuffer;
    int mWritePosition {0};
    
    int mNumInputChannels;
    int mSampleRate;
    
};
