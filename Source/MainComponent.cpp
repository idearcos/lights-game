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
#include <thread>

MainComponent::MainComponent() :
	you_win_screen_{ GetYouWinScreen() }
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
		detachActiveBlocks();
	}
}

void MainComponent::resized()
{
	infoLabel.centreWithSize (getWidth(), 100);
}

void MainComponent::topologyChanged()
{
	infoLabel.setText("Connect a Lightpad Block to start the game!", dontSendNotification);

	// Reset the activeBlock object
	detachActiveBlocks();

	// Get the array of currently connected Block objects from the PhysicalTopologySource
	auto blocks = topologySource.getCurrentTopology().blocks;

	// Iterate over the array of Block objects
	for (auto b : blocks)
	{
		// Find the first Lightpad
		if (b->getType() == Block::Type::lightPadBlock && !lightpad_block_)
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

			if (!isTimerRunning())
			{
				startTimer(300);
			}
        }
        else if(b->getType() == Block::Type::developerControlBlock && !control_block_)
        {
            control_block_ = b;
            
            // Register MainContentComponent as a listener to any control buttons
            for (auto controlButton : control_block_->getButtons())
            {
                controlButton->addListener(this);
            }

			if (game_logic_.GetStageIndex() < 8)
			{
				for (auto controlButton : control_block_->getButtons())
				{
					if ((controlButton->getType() - ControlButton::button0) == game_logic_.GetStageIndex())
					{
						controlButton->setLightColour(LEDColour{ juce::Colours::blue });
					}
					else
					{
						controlButton->setLightColour(LEDColour{ juce::Colours::black });
					}
				}
			}
        }
	}
}


//==============================================================================
void MainComponent::touchChanged(TouchSurface& surface, const TouchSurface::Touch& touch)
{
	if (touch.isTouchEnd) {
		//infoLabel.setText("touch end!", dontSendNotification);
		isTouch = false;
	}
	// Only react when the finger is pressed initially
	if (!touch.isTouchStart) {
		char dbgstr[256];
		sprintf(dbgstr, "pitch: %f, pressure: %f", (touch.x - touch.startX) / getWidth(), touch.z);
		//infoLabel.setText(dbgstr, dontSendNotification);
		audio.pitchChange(1, 300 * (touch.x - touch.startX) / getWidth());
		audio.pressureChange(1, touch.z);
		audio.pitchChange(2, 300 * (touch.x - touch.startX) / getWidth());
		audio.pressureChange(2, touch.z);
		//if (!isTouch && touch.z > 0.5) {
		//	game_logic_.setLedColour(ledColour[ledColourIndex]);
		//	if (++ledColourIndex > 5) ledColourIndex = 0;
		//	isTouch = true;
		//}
		return;
	}

	// Convert to LED coordinates
	const auto led_x_coord = static_cast<size_t>(touch.x / surface.block.getWidth() * 5);
	const auto led_y_coord = static_cast<size_t>(touch.y / surface.block.getHeight() * 5);

	if (auto program = reinterpret_cast<BitmapLEDProgram*>(lightpad_block_->getProgram()))
	{
		if (!game_logic_.IsStageCleared())
		{
			game_logic_.OnLedPressed(led_x_coord, led_y_coord, *program);
			if (game_logic_.IsStageCleared())
			{
				startTimer(100);
			}
		}
		else
		{
			game_logic_.OnLedPressed(led_x_coord, led_y_coord, *program);
		}
	}

	// audio.
	touchAudio(led_x_coord, led_y_coord, touch.z);
}

void MainComponent::touchAudio(int x, int y, float z) {
	// audio.
	char debugstr[256];
	sprintf(debugstr, "touch: (%d, %d) -> %d, %f\n", x, y, getNoteNumberForPad(x, y), z);
	//infoLabel.setText(debugstr, dontSendNotification);
	if (z < 0.02) z = 0.02f;
	audio.noteOn(1, getNoteNumberForPad(x, y), z);
	audio.noteOn(2, 145 - getNoteNumberForPad(x, y), z);
}

void MainComponent::buttonReleased(ControlButton& button, Block::Timestamp)
{
	if (lightpad_block_ == nullptr) return;

	const auto program = reinterpret_cast<juce::BitmapLEDProgram*>(lightpad_block_->getProgram());
	if (program == nullptr) return;

	// Turn off all buttons first
	for (auto &other_button : button.block.getButtons())
	{
		other_button->setLightColour(LEDColour{ juce::Colours::black });
	}

    auto buttonType = button.getType();
    
    if(buttonType >= ControlButton::ButtonFunction::button0 && buttonType <= ControlButton::ButtonFunction::button7){
		stopTimer();
		game_logic_.SetStage(buttonType - ControlButton::ButtonFunction::button0, program);
		button.setLightColour(LEDColour{ juce::Colours::blue });
	}
	audio.allNotesOff();
}

void MainComponent::detachActiveBlocks()
{
	if (lightpad_block_)
	{
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

	if (control_block_)
	{
		for (auto controlButton : control_block_->getButtons())
		{
			controlButton->removeListener(this);
		}

		control_block_ = nullptr;
	}
}

int MainComponent::getNoteNumberForPad(int x, int y) const
{
	//return 60 + (x * 5) + x;
	return 60 + game_logic_.CountLightOn();
}

void MainComponent::timerCallback()
{
	if (!lightpad_block_) return;

	const auto program = reinterpret_cast<juce::BitmapLEDProgram*>(lightpad_block_->getProgram());
	if (program == nullptr) return;

	if (game_logic_.IsStageCleared())
	{
		const auto row_size = you_win_screen_.size() / 15;
		for (int x = 0; x < 15; ++x)
		{
			for (int y = 0; y < 15; ++y)
			{
				auto pixel_offset = you_win_scoll_ + x;
				if (pixel_offset >= row_size) pixel_offset -= row_size;

				pixel_offset += y * row_size;

				program->setLED(x, y, you_win_screen_[pixel_offset]);
			}
		}

		you_win_scoll_ += 1;
		if (you_win_scoll_ >= row_size)
		{
			you_win_scoll_ = 0;
		}
	}
	else
	{
		game_logic_.RefreshWholeScreen(*program);
		stopTimer();
	}
}
