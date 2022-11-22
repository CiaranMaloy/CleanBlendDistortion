/*
  ==============================================================================

    VisualiserWindow.cpp
    Created: 9 Nov 2022 7:41:36pm
    Author:  ciaran maloy

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VisualiserWindow.h"

//==============================================================================
VisualiserWindow::VisualiserWindow(CleanBlendDistortionAudioProcessor& p) : outerBounds(), innerBounds(), audioProcessor(p)
{
    startTimer(20);
}

void VisualiserWindow::timerCallback()
{
    repaint();
}

VisualiserWindow::~VisualiserWindow()
{
}

void VisualiserWindow::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);   // clear the background
    g.setColour(juce::Colours::black);
    g.fillRect(outerBounds);
    g.setColour(juce::Colours::black);
    g.fillRect(innerBounds);
    
    auto output_path = generateAudioPath(innerBounds, 2);
    g.setColour(juce::Colours::white);
    g.strokePath(output_path, juce::PathStrokeType(1.f));
    
    auto input_path = generateAudioPath(innerBounds, 0);
    g.setColour(juce::Colours::powderblue);
    g.strokePath(input_path, juce::PathStrokeType(1.f));
}

void VisualiserWindow::resized()
{
    outerBounds.setBounds(0, 0, getWidth(), getHeight());
    
    // Place visualiser window inside
    auto reduceRatioX = 0.0f;
    auto reduceRatioY = 0.1f;
    auto reduceX = getWidth()*reduceRatioX;
    auto reduceY = getHeight()*reduceRatioY;
    
    innerBounds.setBounds(reduceX, reduceY, getWidth()-2.0*reduceX, getHeight()-2.0*reduceY);
    
}

juce::Path VisualiserWindow::generateRandomPath(juce::Rectangle<float> Rect)
{
        juce::Path randomPath;
        juce::Random r;

        randomPath.startNewSubPath(Rect.getX(), Rect.getY() + Rect.getHeight() * r.nextFloat());

        // draw a random line
        for (auto x = Rect.getX() + 1; x < Rect.getRight(); x += 2)
        {
            randomPath.lineTo(x, Rect.getY() + Rect.getHeight() * r.nextFloat());
        }
    
    return randomPath;
}

juce::Path VisualiserWindow::generateAudioPath(juce::Rectangle<float> Rect, const int CHANNEL)
{
    juce::Path randomPath;
    juce::AudioBuffer<float> displayBuffer = audioProcessor.getBufferForDisplay();
    
        
    const float OFFSET = 0.5;
    const int DOWNSAMPLE = 10;
    float scalingFactor = 2;
    auto* channelData = displayBuffer.getWritePointer(CHANNEL);
    
    randomPath.startNewSubPath(Rect.getX(), Rect.getY() + Rect.getHeight() * (-channelData[0] + OFFSET));

    // draw a random line
    for (int x = Rect.getX()+1; x < Rect.getRight()+1; x += 1)
    {
        //DBG(channelData[x*DOWNSAMPLE]);
        if (scalingFactor > 0.0f)
        {
            randomPath.lineTo(x, Rect.getY() + Rect.getHeight() * (-channelData[x*DOWNSAMPLE]/scalingFactor + OFFSET));
        }
        else
        {
            randomPath.lineTo(x, Rect.getY() + Rect.getHeight() * (-channelData[x*DOWNSAMPLE] + OFFSET));
        }
    }
    
    return randomPath;
}
