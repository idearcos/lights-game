/*
  ==============================================================================

    Stages.h
    Created: 7 Oct 2017 4:56:44pm
    Author:  James Matthews

  ==============================================================================
*/

#pragma once

/**
 Represents a vector of stages
 */
#include<vector>
#include<array>
#include "../JuceLibraryCode/JuceHeader.h"

using GameState = std::array<juce::Colour, 5*5>;
const std::vector<GameState> stages{
    {
        Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
        Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
        Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
        Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
        Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple
    },
    {
        Colours::black, Colours::black, Colours::purple, Colours::purple, Colours::purple,
        Colours::black, Colours::purple, Colours::black, Colours::black, Colours::purple,
        Colours::purple, Colours::black, Colours::black, Colours::purple, Colours::purple,
        Colours::purple, Colours::black, Colours::purple, Colours::black, Colours::black,
        Colours::purple, Colours::purple, Colours::black, Colours::purple, Colours::black
    },
    {
        Colours::black, Colours::black, Colours::purple, Colours::purple, Colours::black,
        Colours::black, Colours::black, Colours::purple, Colours::purple, Colours::purple,
        Colours::black, Colours::purple, Colours::purple, Colours::purple, Colours::black,
        Colours::purple, Colours::purple, Colours::black, Colours::black, Colours::purple,
        Colours::black, Colours::purple, Colours::black, Colours::black, Colours::black
    },
    {
        Colours::purple, Colours::black, Colours::black, Colours::purple, Colours::black,
        Colours::black, Colours::purple, Colours::black, Colours::purple, Colours::black,
        Colours::purple, Colours::purple, Colours::purple, Colours::black, Colours::purple,
        Colours::black, Colours::purple, Colours::black, Colours::black, Colours::black,
        Colours::purple, Colours::purple, Colours::purple, Colours::black, Colours::purple
    }
};
