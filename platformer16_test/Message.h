/*

Systems communicate with each other using messages
Below are enums and structures related to messages
*/

#pragma once

#include "GameObject.h"
#include <vector>
#include "Tilepos.h"

enum MessageType
{
	MESSAGE_COLLISION = 0,
	MESSAGE_DAMAGE,
	MESSAGE_STATETRANSITION
};

struct Message
{
	MessageType m_type;
};

enum CollisionMessageCollisionType
{
	COLLISION_OBJECT = 0,
	COLLISION_WALL
};

enum CollisionMessageWallSide
{
	COL_RIGHT = 1,
	COL_UP = 2,
	COL_LEFT = 4,
	COL_DOWN = 8
};

struct CollisionMessage : Message
{
	CollisionMessageCollisionType collision_type;
	UniqueEntityIdentifier source;
	UniqueEntityIdentifier target;
	int collision_mask;   // see CollisionMessageWallSide
	std::vector<Tilepos>* collided_walls;
	CollisionMessage() { m_type = MESSAGE_COLLISION; collided_walls = 0; }
	~CollisionMessage() { if (collided_walls) delete collided_walls; }
};

struct DamageMessage : Message
{
	UniqueEntityIdentifier source;
	UniqueEntityIdentifier target;
	float damage;
	DamageMessage() { m_type = MESSAGE_DAMAGE; }
};

struct StateTransitionMessage : Message
{
	UniqueEntityIdentifier source;
	int from;
	int to;
	StateTransitionMessage() { m_type = MESSAGE_STATETRANSITION; }
};