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
	topologySource.addListener(this);

	infoLabel.setText("Connect a Lightpad Block to start the game!", dontSendNotification);
	infoLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(infoLabel);

	setSize(600, 600);
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

			// Set bitmap LED program
			lightpad_block_->setProgram(new BitmapLEDProgram(*lightpad_block_));

			// Register MainContentComponent as a listener to the touch surface
			if (auto surface = lightpad_block_->getTouchSurface())
			{
				surface->addListener(this);
			}

			infoLabel.setText("Look at the lightpad block to play the game!", juce::dontSendNotification);
        }
        else if(b->getType() == Block::Type::developerControlBlock)
        {
            control_block_ = b;
            
            // Register MainContentComponent as a listener to any control buttons
            for (auto controlButton : control_block_->getButtons())
            {
                controlButton->addListener(this);
            }
        }
	}
}

//==============================================================================
void MainComponent::touchChanged(TouchSurface& surface, const TouchSurface::Touch& touch)
{
	// Only react when the finger is pressed initially
	if (!touch.isTouchStart) return;

	// Convert to LED coordinates
	const auto led_x_coord = static_cast<size_t>(touch.x / surface.block.getWidth() * 5);
	const auto led_y_coord = static_cast<size_t>(touch.y / surface.block.getHeight() * 5);

	if (auto program = reinterpret_cast<BitmapLEDProgram*>(lightpad_block_->getProgram()))
	{
		game_logic_.OnLedPressed(led_x_coord, led_y_coord, *program);
	}

	// audio.
	audio.noteOn(1, getNoteNumberForPad(touch.x, touch.y), touch.z);
}

void MainComponent::buttonReleased(ControlButton& button, Block::Timestamp)
{
    auto buttonType = button.getType();
    
    if(buttonType >= ControlButton::ButtonFunction::button0 && buttonType <= ControlButton::ButtonFunction::button7){
        game_logic_.SetStage(buttonType - ControlButton::ButtonFunction::button0, *reinterpret_cast<juce::BitmapLEDProgram*>(lightpad_block_->getProgram()));
    }

	audio.allNotesOff();
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

int MainComponent::getNoteNumberForPad(int x, int y) const
{
	auto xIndex = x / 3;
	auto yIndex = y / 3;

	return 60 + ((4 - yIndex) * 5) + xIndex;
}
