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
    startTimer(40);
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
    g.setColour(juce::Colours::plum);
    g.fillRect(innerBounds);
    
    auto path_to_paint = generateAudioPath(innerBounds);
    g.setColour(juce::Colours::black);
    g.strokePath(path_to_paint, juce::PathStrokeType(1.f));
    
}

void VisualiserWindow::resized()
{
    outerBounds.setBounds(0, 0, getWidth(), getHeight());
    
    // Place visualiser window inside
    auto reduceRatioX = 0.1;
    auto reduceRatioY = 0.2;
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

juce::Path VisualiserWindow::generateAudioPath(juce::Rectangle<float> Rect)
{
    juce::Path randomPath;
    juce::AudioBuffer<float> displayBuffer = audioProcessor.getBufferForDisplay();
        
    const int CHANNEL = 1;
    const float OFFSET = 0.5;
    const int DOWNSAMPLE = 5;
    auto* channelData = displayBuffer.getWritePointer(CHANNEL);
    
    randomPath.startNewSubPath(Rect.getX(), Rect.getY() + Rect.getHeight() * (channelData[0] + OFFSET));

    // draw a random line
    for (int x = Rect.getX()+1; x < Rect.getRight()+1; x += 1)
    {
        randomPath.lineTo(x, Rect.getY() + Rect.getHeight() * (channelData[x*DOWNSAMPLE] + OFFSET));
    }
    
    return randomPath;
}
