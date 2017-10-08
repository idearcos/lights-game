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

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <array>
#include <cstdint>
#include "GameLogic.h"
#include "Audio.h"

class MainComponent : public Component,
	public TopologySource::Listener,
	private TouchSurface::Listener,
	private ControlButton::Listener,
	private Timer
{
public:
	MainComponent();
	~MainComponent();

	void resized() override;

	void topologyChanged() override;

	void timerCallback() override;

private:

	int getNoteNumberForPad(int x, int y) const;
	void touchChanged(TouchSurface&, const TouchSurface::Touch&) override;
	void touchAudio(int x, int y, float z);
	void buttonPressed(ControlButton&, Block::Timestamp) override { }
	void buttonReleased(ControlButton&, Block::Timestamp) override;

	void detachActiveBlocks();

private:
	GameLogic game_logic_;

	PhysicalTopologySource topologySource;
	Block::Ptr lightpad_block_;
    Block::Ptr control_block_;

	Label infoLabel;

	Audio audio;

	using Screen = std::vector<juce::Colour>;
	const Screen you_win_screen_;
	size_t you_win_scoll_{ 0 };

	static Screen GetYouWinScreen()
	{
		const auto X = Colours::purple;
		const auto _ = Colours::black;

		return std::vector<juce::Colour>{
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X,_,_,_,_,_,_,_,X,_,_,X,X,X,X,_,_,X,_,_,_,_,X,_,_,_,X,_,_,_,_,_,X,_,_,X,X,X,X,X,_,X,_,_,_,_,X,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X,_,_,_,_,_,X,_,_,X,_,_,_,_,X,_,X,_,_,_,_,X,_,_,_,X,_,_,_,_,_,X,_,_,_,_,X,_,_,_,X,X,_,_,_,X,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X,_,_,_,X,_,_,_,X,_,_,_,_,X,_,X,_,_,_,_,X,_,_,_,X,_,_,_,_,_,X,_,_,_,_,X,_,_,_,X,X,_,_,_,X,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X,_,X,_,_,_,_,X,_,_,_,_,X,_,X,_,_,_,_,X,_,_,_,X,_,_,_,_,_,X,_,_,_,_,X,_,_,_,X,_,X,_,_,X,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X,_,_,_,_,_,X,_,_,_,_,X,_,X,_,_,_,_,X,_,_,_,X,_,_,_,_,_,X,_,_,_,_,X,_,_,_,X,_,X,_,_,X,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X,_,_,_,_,_,X,_,_,_,_,X,_,X,_,_,_,_,X,_,_,_,X,_,_,_,_,_,X,_,_,_,_,X,_,_,_,X,_,_,X,_,X,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X,_,_,_,_,_,X,_,_,_,_,X,_,X,_,_,_,_,X,_,_,_,X,_,_,X,_,_,X,_,_,_,_,X,_,_,_,X,_,_,X,_,X,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X,_,_,_,_,_,X,_,_,_,_,X,_,X,_,_,_,_,X,_,_,_,X,_,X,_,X,_,X,_,_,_,_,X,_,_,_,X,_,_,_,X,X,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,X,_,_,_,_,_,_,X,X,X,X,_,_,_,X,X,X,X,_,_,_,_,_,X,_,_,_,X,_,_,_,X,X,X,X,X,_,X,_,_,_,_,X,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
			_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,
		};

	}

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
	bool isTouch = false;
	const Colour ledColour[6] = {
		Colours::purple, Colours::aqua, Colours::orange, Colours::green, Colours::pink, Colours::yellow
	};
	int ledColourIndex = 0;
};
