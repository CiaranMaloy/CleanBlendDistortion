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
ViewVoltageTransfer::ViewVoltageTransfer(WaveShaping::EffectType type)
{
    mInternalType = type;
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
    
    //auto path_to_paint = generateVoltageTransferPath(innerBounds);
    auto path_to_paint = generateVoltageTransferPath(innerBounds, mInternalType);
    
    g.setColour(mSetColour);
    g.strokePath(path_to_paint, juce::PathStrokeType(1.f));
    
    // paint cross
    juce::Path crossVerticalPath;
    crossVerticalPath.startNewSubPath(innerBounds.getX()+1 + innerBounds.getWidth()/2, innerBounds.getY());
    crossVerticalPath.lineTo(innerBounds.getX()+1 + innerBounds.getWidth()/2, innerBounds.getY() + innerBounds.getHeight());
    g.strokePath(crossVerticalPath, juce::PathStrokeType(1.f));
    
    juce::Path crossHorisontalPath;
    crossHorisontalPath.startNewSubPath(innerBounds.getX(), innerBounds.getY()+innerBounds.getHeight()/2);
    crossHorisontalPath.lineTo(innerBounds.getX() + innerBounds.getWidth(), innerBounds.getY() + innerBounds.getHeight()/2);
    g.strokePath(crossHorisontalPath, juce::PathStrokeType(1.f));
}

void ViewVoltageTransfer::resized()
{
    outerBounds.setBounds(0, 0, getWidth(), getHeight());
    
    // Place visualiser window inside
    auto reduceRatioX = 0.1;
    auto reduceRatioY = 0.1;
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
    int N = 10*(Rect.getRight() - Rect.getX());
    
    juce::Path displayPath;
    WaveShaping waveShapeViewer;
    juce::AudioBuffer<float> displayBuffer = waveShapeViewer.voltageTransferFunction(type, N);
    int CHANNEL = displayBuffer.getNumChannels() - 1;
    
    const int width = Rect.getRight() - Rect.getX();
    const float OFFSET = 0.5;
    const int DOWNSAMPLE = N/width;
    
    const float* channelData = displayBuffer.getReadPointer(CHANNEL);
    
    
    // draw a random line
    float scaling = 0.5;
    float maxValue = displayBuffer.getMagnitude(CHANNEL, 0, displayBuffer.getNumSamples());
    
    displayPath.startNewSubPath(Rect.getX(), Rect.getY() + Rect.getHeight() * (-scaling*channelData[0]/maxValue + OFFSET));
    for (int x = Rect.getX()+1; x < Rect.getRight()+1; x += 1)
    {
        int x_s = x-Rect.getX()-1;
        //DBG(channelData[x*DOWNSAMPLE]);
        displayPath.lineTo(x, Rect.getY() + Rect.getHeight() * (-scaling*channelData[x_s*DOWNSAMPLE]/maxValue + OFFSET));
    }
    
    return displayPath;
}
