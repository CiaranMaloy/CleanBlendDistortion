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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

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
    
    auto path_to_paint = generateVoltageTransferPath(innerBounds);
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
