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
#include "Audio.h"

class MainComponent : public Component,
	public TopologySource::Listener,
	private TouchSurface::Listener,
	private ControlButton::Listener
{
public:
	MainComponent();
	~MainComponent();

	void resized() override;

	void topologyChanged() override;

private:

	int getNoteNumberForPad(int x, int y) const;
	void touchChanged(TouchSurface&, const TouchSurface::Touch&) override;

	void buttonPressed(ControlButton&, Block::Timestamp) override { }
	void buttonReleased(ControlButton&, Block::Timestamp) override;

	void detachActiveBlock();

	void toggleNextColor(size_t x, size_t y);

private:
	enum class GameMode
	{
		BlackAndWhite,
		Tricolor
	} game_mode_{ GameMode::BlackAndWhite };
	std::array<juce::Colour, 5*5> light_colors_;

	PhysicalTopologySource topologySource;
	Block::Ptr lightpad_block_;
    Block::Ptr control_block_;

	Label infoLabel;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)

	Audio audio;
};
