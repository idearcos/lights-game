#include "GameLogic.h"

void GameLogic::OnLedPressed(size_t led_x_coord, size_t led_y_coord, juce::BitmapLEDProgram &program)
{
	if (current_stage_.IsStageClear(game_state_)) { return; }

	++number_of_moves_;

	ToggleNextColor(led_x_coord, led_y_coord, program);

	if (led_x_coord > 0) { ToggleNextColor(led_x_coord - 1, led_y_coord, program); }
	if (led_x_coord < 4) { ToggleNextColor(led_x_coord + 1, led_y_coord, program); }
	if (led_y_coord > 0) { ToggleNextColor(led_x_coord, led_y_coord - 1, program); }
	if (led_y_coord < 4) { ToggleNextColor(led_x_coord, led_y_coord + 1, program); }
}

void GameLogic::ToggleNextColor(size_t x, size_t y, juce::BitmapLEDProgram &program)
{
	const auto light_index = 5 * y + x;

	switch (game_mode_)
	{
	case GameMode::BlackAndWhite:
		if (game_state_[light_index] == juce::Colours::black)
		{
			game_state_[light_index] = onColour;
		}
		else
		{
			game_state_[light_index] = juce::Colours::black;
		}
		break;
	default:
		break;
	}

	SetLedColor(x, y, game_state_[light_index], program);
}

void GameLogic::SetLedColor(size_t x, size_t y, juce::Colour color, juce::BitmapLEDProgram &program) const
{
	for (int ii = 0; ii < 3; ++ii)
	{
		for (int jj = 0; jj < 3; ++jj)
		{
			program.setLED(3 * x + ii, 3 * y + jj, color);
		}
	}
}

void GameLogic::SetStage(size_t new_stage_index, juce::BitmapLEDProgram* program)
{
	current_stage_ = Stage::GetStage(new_stage_index);
	stage_index_ = new_stage_index;

	game_state_ = current_stage_.initial_state;

	if (program)
	{
		RefreshWholeScreen(*program);
	}
}

int GameLogic::CountLightOn() const
{
	int count = 0;
	for (int i = 0; i < 16; ++i) {
		if (game_state_[i] != Colours::black) {
			++count;
		}
	}
	return count;
}

void GameLogic::setLedColour(Colour col)
{
	onColour = col;
}

void GameLogic::RefreshWholeScreen(juce::BitmapLEDProgram &program)
{
	for (int x = 0; x < 5; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			SetLedColor(x, y, game_state_[5 * y + x], program);
		}
	}
}

GameLogic::Stage GameLogic::Stage::GetStage(size_t stage_index)
{
	Stage stage;
	stage.IsStageClear = [](const GameState &game_state) { return std::all_of(game_state.cbegin(), game_state.cend(), [](const juce::Colour& color) { return color == juce::Colours::black; }); };

	switch (stage_index)
	{
	case 1:
		stage.initial_state = {
			Colours::purple, Colours::purple, Colours::black, Colours::black, Colours::black,
			Colours::purple, Colours::black, Colours::purple, Colours::black, Colours::black,
			Colours::black, Colours::purple, Colours::purple, Colours::purple, Colours::black,
			Colours::black, Colours::black, Colours::purple, Colours::black, Colours::purple,
			Colours::black, Colours::black, Colours::black, Colours::purple, Colours::purple
		};
		stage.target = 0;
		break;
	case 2:
		stage.initial_state = {
			Colours::black, Colours::black, Colours::purple, Colours::purple, Colours::purple,
			Colours::black, Colours::purple, Colours::black, Colours::black, Colours::purple,
			Colours::purple, Colours::black, Colours::black, Colours::purple, Colours::purple,
			Colours::purple, Colours::black, Colours::purple, Colours::black, Colours::black,
			Colours::purple, Colours::purple, Colours::black, Colours::purple, Colours::black
		};
		stage.target = 0;
		break;
	case 3:
		stage.initial_state = {
			Colours::black, Colours::black, Colours::purple, Colours::purple, Colours::black,
			Colours::black, Colours::black, Colours::purple, Colours::purple, Colours::purple,
			Colours::black, Colours::purple, Colours::purple, Colours::purple, Colours::black,
			Colours::purple, Colours::purple, Colours::black, Colours::black, Colours::purple,
			Colours::black, Colours::purple, Colours::black, Colours::black, Colours::black
		};
		stage.target = 0;
		break;
	case 4:
		stage.initial_state = {
			Colours::purple, Colours::black, Colours::black, Colours::purple, Colours::black,
			Colours::black, Colours::purple, Colours::black, Colours::purple, Colours::black,
			Colours::purple, Colours::purple, Colours::purple, Colours::black, Colours::purple,
			Colours::black, Colours::purple, Colours::black, Colours::black, Colours::black,
			Colours::purple, Colours::purple, Colours::purple, Colours::black, Colours::purple
		};
		stage.target = 0;
		break;
	case 5:
		stage.initial_state = {
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple
		};
		stage.target = 0;
		break;
	case 6:
		stage.initial_state = {
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple
		};
		stage.target = 0;
		break;
	case 7:
		stage.initial_state = {
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple,
			Colours::purple, Colours::purple, Colours::purple, Colours::purple, Colours::purple
		};
		stage.target = 0;
		break;
	default:
		stage.initial_state.fill(Colours::black);
		stage.IsStageClear = [](const GameState &game_state) { return false; };
		break;
	}

	return stage;
}