#pragma once

#include <array>
#include "../JuceLibraryCode/JuceHeader.h"

class GameLogic final
{
public:
	GameLogic()
	{
		game_state_.fill(juce::Colours::black);
	}
	~GameLogic() = default;

	void OnLedPressed(size_t led_x_coord, size_t led_y_coord, juce::BitmapLEDProgram &program);

	void SetStage(size_t new_stage_index, juce::BitmapLEDProgram &program);

	int CountLightOn() const;

private:
	void ToggleNextColor(size_t x, size_t y, juce::BitmapLEDProgram &program);
	void SetLedColor(size_t x, size_t y, juce::Colour color, juce::BitmapLEDProgram &program) const;

	enum class GameMode
	{
		BlackAndWhite,
		Tricolor
	};

private:
	GameMode game_mode_{ GameMode::BlackAndWhite };

	using GameState = std::array<juce::Colour, 5 * 5>;
	GameState game_state_;

	size_t number_of_moves_{ 0 };

private:
	const std::vector<GameState> stages_{
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
};
