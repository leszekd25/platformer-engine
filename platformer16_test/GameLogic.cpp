#include "GameLogic.h"

#include <iostream>

GameLogic::GameLogic()
{
}


GameLogic::~GameLogic()
{
}

// Resolves game events and redirects handling of events to respective systems
void GameLogic::ProcessMessages()
{
	Message* msg;
	while (!game_state.messages.empty())
	{
		msg = game_state.messages.front();
		game_state.messages.pop();
		switch (msg->m_type)
		{
		case MESSAGE_COLLISION:
			collision_resolve_quasi_system.messages.push((CollisionMessage*)msg);
			break;
		case MESSAGE_DAMAGE:
			damage_resolve_quasi_system.messages.push((DamageMessage*)msg);
			break;
		case MESSAGE_STATETRANSITION:
			state_machine_system.messages.push((StateTransitionMessage*)msg);
			break;
		default:
			delete msg;
			break;
		}
	}
}

// A single game step. Order of systems determines order of actions performed within a single frame.
// Refer to each system header to see what it does.
int GameLogic::Step()
{
	game_state.timer.UpdateTime();

	input_system.Step(game_state);
	if (game_state.quit_game)
		return 1;

	movement_system.Step(game_state); ProcessMessages();

	collision_detection_system.Step(game_state); ProcessMessages();

	collision_resolve_quasi_system.Resolve(game_state); ProcessMessages();

	damage_resolve_quasi_system.Resolve(game_state); ProcessMessages();

	animation_system.Step(game_state); ProcessMessages();

	state_machine_system.Resolve(game_state); ProcessMessages();

	rendering_system.Step(game_state);

	unsigned int new_time = game_state.timer.GetTicks() + game_state.ticks_per_frame;
	int delay = new_time - SDL_GetTicks();
	if (delay > 0)
		SDL_Delay(delay);

	return 0;
}