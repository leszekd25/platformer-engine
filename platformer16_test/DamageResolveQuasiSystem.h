/*

Damage resolving system - it calculates and deals damage to any entity it is told to
It also can remove entities from the game if needed
*/


#pragma once

#include <vector>
#include <queue>
#include "GameState.h"

class DamageResolveQuasiSystem
{
public:
	std::queue<DamageMessage*> messages;
	DamageResolveQuasiSystem()
	{
		messages = std::queue<DamageMessage*>();
	};
	~DamageResolveQuasiSystem()
	{
		while (!messages.empty())
		{
			delete messages.front();
			messages.pop();
		}
	};
	void Resolve(GameState& gs)
	{
		std::vector<GameObject>& entities = gs.entities;
		std::vector<DamageableComponent>& damageables = gs.damageables;

		DamageMessage* message;
		UniqueEntityIdentifier source;
		UniqueEntityIdentifier target;
		float damage;
		while (!messages.empty())
		{
			message = messages.front();
			messages.pop();
			// extract info and delete the message
			source = message->source;
			target = message->target;
			damage = message->damage;
			delete message;
			// only check for target validity - source object could have died, so no biggie
			if (!gs.EntityIsValid(target))
				continue;

			// the entity in question for sure has damageable component, no need to check it
			damageables[target.entity_id].health -= damage;
			if (damageables[target.entity_id].health <= 0)
			{
				// kill target
				gs.FreeEntity(target.entity_id);
			}
		}
	}
};