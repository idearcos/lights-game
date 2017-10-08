#pragma once

#include <array>
#include "../JuceLibraryCode/JuceHeader.h"

class GameLogic final
{
public:
	GameLogic()
	{
		game_state_.fill(juce::Colours::black);
		current_stage_ = Stage::GetStage(0);
	}
	~GameLogic() = default;

	void OnLedPressed(size_t led_x_coord, size_t led_y_coord, juce::BitmapLEDProgram &program);
	bool IsStageCleared() const { return current_stage_.IsStageClear(game_state_); }

	void SetStage(size_t new_stage_index, juce::BitmapLEDProgram &program);

	int CountLightOn() const;
	void setLedColour(Colour col);

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
	size_t stage_index_{ 1 };
	Colour onColour = Colours::purple; // purple

private:
	struct Stage
	{
		GameState initial_state;
		size_t target;

		std::function<bool(const GameState &game_state)> IsStageClear;

		static Stage GetStage(size_t stage_index);
	} current_stage_;
};
