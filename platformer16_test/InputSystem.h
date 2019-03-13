/*

Input system processes all events passed by SDL event system
If any entity is allowed to take input, the system might trigger its event callback functions
After message is processed, it's passed to UI manager for UI interaction
*/

#pragma once

#include <vector>
#include "GameState.h"
#include "sol.hpp"

class InputSystem
{
public:
	InputSystem() {};
	~InputSystem() {};
	void Step(GameState& gs)
	{
		std::vector<GameObject>& entities = gs.entities;
		std::vector<InputComponent>& inputs = gs.inputs;
		int last_active_entity = gs.last_active_entity;

		SDL_Event event;
		sol::function* func;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				gs.quit_game = true;
				break;
			}
			switch (event.type)
			{
			// key pressed on this frame
			case SDL_KEYDOWN:
			{
				gs.keys_pressed[event.key.keysym.scancode] = true;
				for (int i = 0; i <= last_active_entity; i++)
				{
					if (!entities[i].active)
						continue;
					if (entities[i].components[C_INPUT])
					{
						func = inputs[i].functions[INPUT_KEY_PRESS];
						if (func!=0)
							(*func)(&gs, i, event.key.keysym.scancode);
					}
				}
				break;
			}
			// key released on this frame
			case SDL_KEYUP:
			{
				gs.keys_pressed[event.key.keysym.scancode] = false;
				for (int i = 0; i <= last_active_entity; i++)
				{
					if (!entities[i].active)
						continue;
					if (entities[i].components[C_INPUT])
					{
						func = inputs[i].functions[INPUT_KEY_RELEASE];
						if (func != 0)
							(*func)(&gs, i, event.key.keysym.scancode);
					}
				}
				break;
			}
			default:
				break;
			}

			// see UIManager
			gs.ui_manager->resolve_event(&event);
		}
	}
};

