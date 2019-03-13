#pragma once

#include <vector>
#include "sol.hpp"
#include "GameState.h"

class AnimationSystem
{
public:
	AnimationSystem() {};
	~AnimationSystem() {};
	void Step(GameState& gs)
	{
		std::vector<GameObject>& entities = gs.entities;
		std::vector<DrawableComponent>& drawables = gs.drawables;
		std::vector<AnimationComponent>& animations = gs.animations;
		std::vector<ControlComponent>& controls = gs.controls;
		std::vector<StateMachineComponent>& statemachines = gs.statemachines;
		std::vector<TimerComponent>& timers = gs.timers;
		Time& time = gs.timer;
		int last_active_entity = gs.last_active_entity;
		int img_count;

		float current_time = time.GetTime();
		float delta_time = time.GetDeltaTime();
		for (int i = 0; i <= last_active_entity; i++)
		{
			if (!entities[i].active)
				continue;
			if ((entities[i].components[C_DRAWABLE]) && (entities[i].components[C_ANIMATION]))  // optimize forther soon
			{
				if ((!animations[i].repeat) && (animations[i].current_image == drawables[i].sprite->image_count))
					continue;
				while (animations[i].next_frame_time < current_time)
				{
					animations[i].next_frame_time += animations[i].time_per_frame;
					animations[i].current_image++;
					if (animations[i].current_image == drawables[i].sprite->image_count)
					{
						if (animations[i].repeat)
							animations[i].current_image = 0;

						(*(animations[i].on_anim_end))(&gs, i);

						if (!animations[i].repeat)
							break;
					}
				}
			}
			if (entities[i].components[C_CONTROL])
			{
				if (controls[i].step_function)
					(*(controls[i].step_function))(&gs, i);
			}
			if (entities[i].components[C_STATEMACHINE])
			{
				int index = statemachines[i].current_state_index;
				(*(statemachines[i].allowed_states))[index].f(&gs, i);
			}
			if (entities[i].components[C_TIMER])
			{
				for (int j = 0; j < timers[i].max_timers; j++)
				{
					if ((timers[i].timers[j].active) && (timers[i].timers[j].time <= current_time))
					{
						timers[i].timers[j].active = false;
						timers[i].timers[j].callback(&gs, i);
					}
				}
			}
		}
	}
};