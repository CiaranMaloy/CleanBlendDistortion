/*
  ==============================================================================

    CircularBuffer.cpp
    Created: 17 Nov 2022 4:54:41pm
    Author:  ciaran maloy

  ==============================================================================
*/

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
{
    
}

CircularBuffer::setConstants(const int numSeconds, const int numInputChannels, const int sampleRate, const int samplesPerBlock)
{
    mNumInputChannels = numInputChannels;
    mSampleRate = sampleRate;
    
    const int circBufferSize = numSeconds * (sampleRate + samplesPerBlock);
    mCircBuffer.setSize(numInputChannels, circBufferSize);
}

// ====== Circular Buffer ======
void CircularBuffer::fillCircularBuffer(int channel, const int bufferLength, const float* bufferData)
{
    // What do we do once the delay buffers hae reached the end of their length (it should wrap back around to the front)
    // 1. copy the data from the main buffer to the delay buffer
    const int circBufferLength = mCircBuffer.getNumSamples();
    const float* cleanCircularBufferData = mCircBuffer.getReadPointer(channel);
    
    if (circBufferLength > bufferLength + mWritePosition)
    {
        mCircBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, 1.0, 1.0);
    }
    else
    {
        const int bufferRemaining = circBufferLength - mWritePosition;

        mCircBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, 1.0, 1.0);

        // now we've come to the end, we need to put the rest of the data back at the start of the buffer
        mCircBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength-bufferRemaining, 1.0, 1.0);
    }
    
    // TODO: When the buffer is full, copy into a new object that can be called whenever, like a left and right
}

void CircularBuffer::updateWritePosition()
{
    mWritePosition++;
    mWritePosition %= mCircBuffer.getNumSamples();
}

juce::AudioBuffer<float> CircularBuffer::getBufferForDisplay()
{
    int N = 10000;
    
    juce::AudioBuffer<float> bufferToReturn(mNumInputChannels, N);
    
    for (int channel = 0; channel < mNumInputChannels; ++channel)
    {
        if (mWritePosition-N-1 > 0)
        {
            bufferToReturn.copyFrom(channel, 0, mCircBuffer, channel, mWritePosition-N-1, N);
        }
        else
        {
            bufferToReturn.copyFrom(channel, 0, mCircBuffer, channel, bufferToReturn.getNumSamples()-(N-mWritePosition), bufferToReturn.getNumSamples());
            bufferToReturn.copyFrom(channel, 0, mCircBuffer, channel, 0, mWritePosition-1);
        }
    }
    return bufferToReturn;
}

