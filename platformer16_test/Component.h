/*

ECS's one of two most important parts: components
Components are simple blocks of data, to be manipulated by systems.
Below are enums and components definitions, manipulated by logic found in systems.
*/

#pragma once

#include <SDL.h>
#include <tuple>
#include <unordered_map>
#include <cassert>
#include "sol.hpp"
#include "Sprite.h"
#include "Vector.h"
#include "Rect.h"
#include "Color.h"

enum ComponentType
{
	C_TRANSFORM = 0,
	C_MOVABLE,
	C_DRAWABLE,
	C_ANIMATION,
	C_COLLIDABLE,
	C_DAMAGEABLE,
	C_DAMAGING,
	C_PHYSICS,
	C_INPUT,
	C_CONTROL,
	C_STATEMACHINE,
	C_TIMER,
	C_MAX
};

enum WorldState
{
	ON_GROUND = 0,
	IN_AIR
};

enum InputEventType
{
	INPUT_KEY_PRESS = 0,
	INPUT_KEY_RELEASE,
	INPUT_MAX
};

enum ControlSide
{
	SIDE_PLAYER = 0,
	SIDE_NEUTRAL,
	SIDE_ENEMY
};

enum StateMachineStates
{
	STATE_DEAD = -1,
	STATE_IDLE = 0
};

struct LuaEntityState
{
	int state_id;
	sol::function f;
	LuaEntityState() { f = 0; }
	LuaEntityState(int s, sol::function _f) : state_id(s), f(_f) {}
	~LuaEntityState() {}
};

struct CallbackTimer
{
	bool active;
	float time;
	sol::function callback;
	CallbackTimer() { active = false; }
};

// note: angle and scale is not used for now
struct TransformComponent
{
	Vector position;
	float angle;
	Vector scale;
	Vector previous_position;
};

struct MovableComponent
{
	Vector speed;
	Vector acceleration;
};

// note: color is not used for now
struct DrawableComponent
{
	Sprite* sprite;
	Color color;
};

struct AnimationComponent
{
	int current_image;
	float next_frame_time;  // seconds
	float time_per_frame;   // seconds
	bool repeat;            // animation repeats itself if repeat is true
	sol::function* on_anim_end;  // when animation ends
	AnimationComponent() { on_anim_end = 0; }
	~AnimationComponent() { if (on_anim_end) delete on_anim_end; }
};

struct CollidableComponent
{
	unsigned int collision_mask;   // collision flags
								   //two entities can collide with each other if bitwise AND of its collision masks is not zero
	Rect bbox;
};

struct DamageableComponent
{
	unsigned int damagedby_mask;   // damage flags
								   // a damaging can damage a damageabe if bitwise AND of its damage masks is not zero
	float health;                  // if drops below zero, it's destroyed
	float max_health;
};

struct DamagingComponent
{
	unsigned int damages_mask;     // damage flags
								   // a damaging can damage a damageabe if bitwise AND of its damage masks is not zero
	float damage;
};

// note: only world_state is used for now
struct PhysicsComponent
{
	WorldState world_state;        // see WorldState enum
	bool override_control;         // if overriden, the entity can't be manipulated by anything other than physics
	int override_control_timer;    // a timestamp after which control is returned
	float mass;
};

struct InputComponent
{
	sol::function* functions[INPUT_MAX];    // Lua functions connected to each input event
	InputComponent() { for (int i = 0; i < INPUT_MAX; i++) functions[i] = 0; }
	~InputComponent() { for (int i = 0; i < INPUT_MAX; i++) if(functions[i] != 0) delete functions[i]; }
};

struct ControlComponent
{
	sol::function* step_function;
	ControlSide side;
	sol::table lua_values;
	ControlComponent() { step_function = 0; }
	~ControlComponent() { if (step_function != 0) delete step_function; }
};

// note: this component is to be handled real-time, no batching, no queueing
// therefore, it doesn't have to be treated like any other component - it can have its own logic
// hopefully it won't be used too often...
struct StateMachineComponent
{
private:
	// returns arry index of the given state, throws error if no index found
	int GetStateIndex(int state)
	{
		for (int i = 0; i < allowed_states->size(); i++)
			if ((*allowed_states)[i].state_id == state)
				return i;
		assert(false);
	}
public:
	int current_state;
	int current_state_index;
	std::vector<LuaEntityState>* allowed_states;
	std::vector<std::vector<sol::function*>>* state_transitions;   // matrix of state transitions

	StateMachineComponent() 
	{
		allowed_states = 0; 
		state_transitions = 0; 
	}
	~StateMachineComponent() 
	{ 
		if (allowed_states)
		{ 
			for (int i = 0; i < allowed_states->size(); i++)
			{
				for (int j = 0; j < allowed_states->size(); j++)
				{
					if (((*state_transitions)[i])[j] != 0)
						delete ((*state_transitions)[i])[j];
				}
			}
			delete state_transitions;
			delete allowed_states;
		} 
	}

	// adds new state to the pool, with given step function
	void AddState(int state, sol::function f)
	{
		if (!allowed_states)
			allowed_states = new std::vector<LuaEntityState>();
		allowed_states->push_back(LuaEntityState(state, f));
	}

	// sets current state to state
	void SetState(int state)
	{
		current_state = state;
		current_state_index = GetStateIndex(state);
	}

	// called after all states are set in stone, creates and resizes arrays so they don't take too much space
	void InitTransitions()
	{
		// retrieves number of possible states, and resizes all remaining arrays
		// assumes no transitions were assigned yet
		int s = allowed_states->size();
		state_transitions = new std::vector<std::vector<sol::function*>>();
		state_transitions->resize(s);
		for (int i = 0; i < s; i++)
		{
			(*state_transitions)[i].resize(s);
			for (int j = 0; j < s; j++)
				(*state_transitions)[i][j] = 0;
		}
	}

	// assumes init was already done
	// adds new transition from one state to another, and sets a callback function when the transition happens
	void AddTransition(int from, int to, sol::function* f)
	{
		((*state_transitions)[GetStateIndex(from)])[GetStateIndex(to)] = f;
	}

	sol::function* GetTransition(int from, int to)
	{
		return ((*state_transitions)[GetStateIndex(from)])[GetStateIndex(to)];
	}
};

struct TimerComponent
{
	int max_timers;
	CallbackTimer timers[16];
};