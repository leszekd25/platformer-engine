/*

GameObject in its essence is just a number.
Here it also contains information on whether it's active or not, and which of its components are active.

UniqueEntityIdentifier is an important structure, it allows for determining with 100% certainty if a given
entity exists.
*/

#pragma once

#include "Component.h"

const int MAX_ENTITIES = 2048;

struct GameObject
{
	int id;                    // if an object is destroyed and another takes its place while some operations on previous
	                           // object is not finished, this will allow double-checking if the object still matches
	bool active;
	bool components[C_MAX];
};

struct UniqueEntityIdentifier
{
	int entity_id;
	int gameobject_id;
	UniqueEntityIdentifier() : entity_id(-1), gameobject_id(-1) {}
	UniqueEntityIdentifier(int e_id, int g_id) : entity_id(e_id), gameobject_id(g_id) {}
	UniqueEntityIdentifier& operator=(const UniqueEntityIdentifier& other)
	{
		if (this != &other) 
		{
			entity_id = other.entity_id;
			gameobject_id = other.gameobject_id;
		}
		return *this;
	}
};