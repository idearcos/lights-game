/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "MainComponent.h"

MainComponent::MainComponent()
{
    // Register MainContentComponent as a listener to the PhysicalTopologySource object
    topologySource.addListener (this);

    infoLabel.setText ("Connect a Lightpad Block to start the game!", dontSendNotification);
    infoLabel.setJustificationType (Justification::centred);
    addAndMakeVisible (infoLabel);

    setSize (600, 600);
}

MainComponent::~MainComponent()
{
	if (lightpad_block_ != nullptr)
	{
		detachActiveBlock();
	}
}

void MainComponent::resized()
{
	infoLabel.centreWithSize (getWidth(), 100);
}

void MainComponent::topologyChanged()
{
	infoLabel.setVisible(true);

	// Reset the activeBlock object
	detachActiveBlock();

	// Get the array of currently connected Block objects from the PhysicalTopologySource
	auto blocks = topologySource.getCurrentTopology().blocks;

	// Iterate over the array of Block objects
	for (auto b : blocks)
	{
		// Find the first Lightpad
		if (b->getType() == Block::Type::lightPadBlock)
		{
			lightpad_block_ = b;

			lightpad_block_->setProgram(new BitmapLEDProgram(*lightpad_block_));

			// Register MainContentComponent as a listener to the touch surface
			if (auto surface = lightpad_block_->getTouchSurface())
			{
				surface->addListener(this);
			}

			// Register MainContentComponent as a listener to any buttons
			for (auto button : lightpad_block_->getButtons())
			{
				button->addListener(this);
			}

			infoLabel.setText("Look at the lightpad block to play the game!", juce::dontSendNotification);

			break;
		}
	}
}

//==============================================================================
void MainComponent::touchChanged(TouchSurface&, const TouchSurface::Touch& touch)
{
	// Game logic
}

void MainComponent::buttonReleased(ControlButton& button, Block::Timestamp)
{
	// Stage select, etc
}

void MainComponent::detachActiveBlock()
{
	if (!lightpad_block_) return;

	if (auto surface = lightpad_block_->getTouchSurface())
	{
		surface->removeListener(this);
	}

	for (auto button : lightpad_block_->getButtons())
	{
		button->removeListener(this);
	}

	lightpad_block_ = nullptr;
}
