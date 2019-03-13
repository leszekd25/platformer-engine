/*

A timer class, which keeps track of current in-game time in ticks and seconds, and calculates delta time for purposes
of game simulation
*/

#pragma once

#include <SDL_timer.h>

class Time
{
	unsigned int ticks_elapsed;    //miliseconds
	float current_time;
	float delta_time;
public:
	Time();
	~Time();
	void UpdateTime();
	float GetTime() const;
	float GetDeltaTime() const;
	unsigned int GetTicks() const;
};

