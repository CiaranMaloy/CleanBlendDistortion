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

void CircularBuffer::setConstants(const int numSeconds, const int numInputChannels, const int sampleRate, const int samplesPerBlock)
{
    mNumInputChannels = numInputChannels;
    mSampleRate = sampleRate;
    
    const int circBufferSize = numSeconds * (sampleRate + samplesPerBlock);
    mCircBuffer.setSize(numInputChannels, circBufferSize);
}

// ====== Circular Buffer ======
void CircularBuffer::fillCircularBuffer(int channel, const int bufferLength, const float* bufferData)
{
    const int circBufferLength = mCircBuffer.getNumSamples();
    
    if (circBufferLength > bufferLength + mWritePosition)
    {
        mCircBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferLength, 1.0, 1.0);
    }
    else
    {
        const int bufferRemaining = circBufferLength - mWritePosition;

        if (bufferRemaining > 0)
        {
            mCircBuffer.copyFromWithRamp(channel, mWritePosition, bufferData, bufferRemaining, 1.0, 1.0);
        }
        mCircBuffer.copyFromWithRamp(channel, 0, bufferData, bufferLength-bufferRemaining, 1.0, 1.0);
    }
}

void CircularBuffer::updateWritePosition(const int bufferLength)
{
    mWritePosition += bufferLength;
    mWritePosition %= mCircBuffer.getNumSamples();
}

juce::AudioBuffer<float> CircularBuffer::getBufferForDisplay()
{
    int N = 10000;
    
    juce::AudioBuffer<float> bufferToReturn(mNumInputChannels, N);
    
    jassert(mWritePosition-1 <= mCircBuffer.getNumSamples());
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

