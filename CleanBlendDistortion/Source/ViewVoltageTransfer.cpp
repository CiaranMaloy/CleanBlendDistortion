/*
  ==============================================================================

    ViewVoltageTransfer.cpp
    Created: 20 Nov 2022 10:10:23am
    Author:  ciaran maloy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ViewVoltageTransfer.h"

//==============================================================================
ViewVoltageTransfer::ViewVoltageTransfer()
{
}

ViewVoltageTransfer::~ViewVoltageTransfer()
{
}

void ViewVoltageTransfer::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);   // clear the background
    g.setColour(juce::Colours::black);
    g.fillRect(outerBounds);
    g.setColour(juce::Colours::black);
    g.fillRect(innerBounds);
    
    auto path_to_paint = generateVoltageTransferPath(innerBounds, WaveShaping::EffectType::fullWaveRectifier);
    g.setColour(juce::Colours::white);
    g.strokePath(path_to_paint, juce::PathStrokeType(1.f));
}

void ViewVoltageTransfer::resized()
{
    outerBounds.setBounds(0, 0, getWidth(), getHeight());
    
    // Place visualiser window inside
    auto reduceRatioX = 0.1;
    auto reduceRatioY = 0.2;
    auto reduceX = getWidth()*reduceRatioX;
    auto reduceY = getHeight()*reduceRatioY;
    
    innerBounds.setBounds(reduceX, reduceY, getWidth()-2.0*reduceX, getHeight()-2.0*reduceY);
}

juce::Path ViewVoltageTransfer::generateVoltageTransferPath(juce::Rectangle<float> Rect)
{
        juce::Path displayPath;
        juce::Random r;

        displayPath.startNewSubPath(Rect.getX(), Rect.getY() + Rect.getHeight() * r.nextFloat());

        // draw a random line
        for (auto x = Rect.getX() + 1; x < Rect.getRight(); x += 2)
        {
            displayPath.lineTo(x, Rect.getY() + Rect.getHeight() * r.nextFloat());
        }
    
    return displayPath;
}

juce::Path ViewVoltageTransfer::generateVoltageTransferPath(juce::Rectangle<float> Rect, WaveShaping::EffectType type)
{
    juce::AudioBuffer<float> mDisplayBuffer;
    int CHANNEL = 0;
    int NCHANNELS = 1;
    int N = 500;
    mDisplayBuffer.setSize(NCHANNELS, N);
    juce::Path displayPath;
    
    // === writer -1 to 1 in the buffer
    for (int sample = 0; sample < mDisplayBuffer.getNumSamples(); sample++)
    {
        float input = (static_cast<float>(sample) - static_cast<float>(N)/2.0f)/(static_cast<float>(N)/2.0f);
        mDisplayBuffer.setSample(CHANNEL, sample, input);
    }
    // ======
    // ====== Put the audio throught the effect
    
    switch (type)
    {
        case WaveShaping::EffectType::distortion:
            WaveShaping::process(WaveShaping::EffectType::distortion, mDisplayBuffer, CHANNEL);
            WaveShaping::process(WaveShaping::EffectType::fuzz, mDisplayBuffer, CHANNEL);
            break;
        
        case WaveShaping::EffectType::fuzz:
            WaveShaping::process(WaveShaping::EffectType::fuzz, mDisplayBuffer, CHANNEL);
            break;
            
        case WaveShaping::EffectType::fullWaveRectifier:
            WaveShaping::process(WaveShaping::EffectType::fullWaveRectifier, mDisplayBuffer, CHANNEL);
            break;
    }
    
    const int width = Rect.getRight() - Rect.getX();
    const float OFFSET = 0.5;
    const int DOWNSAMPLE = N/width;
    auto* channelData = mDisplayBuffer.getReadPointer(CHANNEL);
    
    displayPath.startNewSubPath(Rect.getX(), Rect.getY() + Rect.getHeight() * (channelData[0] + OFFSET));

    // draw a random line
    for (int x = Rect.getX()+1; x < Rect.getRight()+1; x += 1)
    {
        //DBG(channelData[x*DOWNSAMPLE]);
        displayPath.lineTo(x, Rect.getY() + Rect.getHeight() * (channelData[x*DOWNSAMPLE] + OFFSET));
    }
    
    return displayPath;
}
