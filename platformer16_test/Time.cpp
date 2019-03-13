#include "Time.h"



Time::Time()
{
	ticks_elapsed = SDL_GetTicks();
}


Time::~Time()
{
}

// is called once per frame, see GameLogic::Step()
// updates current time in seconds and ticks, and calculates delta time of previous frame to be used in the current frame
void Time::UpdateTime()
{
	unsigned int new_time = SDL_GetTicks();
	current_time = new_time / 1000.0f;
	delta_time = (new_time - ticks_elapsed) / 1000.0f;
	ticks_elapsed = new_time;
}

float Time::GetTime() const
{
	return current_time;
}

float Time::GetDeltaTime() const
{
	return delta_time;
}

unsigned int Time::GetTicks() const
{
	return ticks_elapsed;
}