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

class MainComponent : public Component,
	public TopologySource::Listener,
	private TouchSurface::Listener,
	private ControlButton::Listener
{
public:
	MainComponent();
	~MainComponent();

	void resized() override;

	/** Overridden from TopologySource::Listener. Called when the topology changes */
	void topologyChanged() override;

private:
	/** Overridden from TouchSurface::Listener. Called when a Touch is received on the Lightpad */
	void touchChanged(TouchSurface&, const TouchSurface::Touch&) override;

	/** Overridden from ControlButton::Listener. Called when a button on the Lightpad is pressed */
	void buttonPressed(ControlButton&, Block::Timestamp) override { }

	/** Overridden from ControlButton::Listener. Called when a button on the Lightpad is released */
	void buttonReleased(ControlButton&, Block::Timestamp) override;

	/** Removes TouchSurface and ControlButton listeners and sets activeBlock to nullptr */
	void detachActiveBlock();

	/** Sets an LED on the Lightpad for a given touch co-ordinate and pressure */
	void drawLED(uint32 x0, uint32 y0, float z, Colour drawColour);

	/** Redraws the LEDs on the Lightpad from the activeLeds array */
	void redrawLEDs();

	PhysicalTopologySource topologySource;
	Block::Ptr lightpad_block_;

	Label infoLabel;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
