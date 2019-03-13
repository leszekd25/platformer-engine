/*

Movement system will move each entity according to its speed and accceleration
It also knows whether the entity is in air, so it can apply gravity if needed
It's the first system activated each frame
*/

#pragma once

#include <vector>
#include "GameState.h"
#include <iostream>

class MovementSystem
{
public:
	MovementSystem() {};
	~MovementSystem() {};
	void Step(GameState& gs)
	{
		std::vector<GameObject>& entities = gs.entities;
		std::vector<TransformComponent>& transforms = gs.transforms;
		std::vector<MovableComponent>& movables = gs.movables;
		std::vector<PhysicsComponent>& physicss = gs.physicss;
		WorldProperties& world_properties = gs.world_properties;
		Time& time = gs.timer;
		int last_active_entity = gs.last_active_entity;

		float delta_time = time.GetDeltaTime();

		for (int i = 0; i <= last_active_entity; i++)
		{
			if (!entities[i].active)
				continue;
			if ((entities[i].components[C_TRANSFORM]) && (entities[i].components[C_MOVABLE]))  // optimize further
			{
				if (entities[i].components[C_PHYSICS])
				{
					if (physicss[i].world_state == IN_AIR)
						movables[i].speed.y += world_properties.gravity_strength * delta_time;
				}
				movables[i].speed += movables[i].acceleration*delta_time;
				transforms[i].previous_position = transforms[i].position;
				transforms[i].position += movables[i].speed*delta_time;
			}
		}
	}
};

