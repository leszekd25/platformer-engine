#pragma once

#include <vector>
#include <queue>
#include "sol.hpp"
#include "GameState.h"

class StateMachineSystem
{
public:
	std::queue<StateTransitionMessage*> messages;
	StateMachineSystem()
	{
		messages = std::queue<StateTransitionMessage*>();
	};
	~StateMachineSystem()
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
		std::vector<StateMachineComponent>& statemachines = gs.statemachines;

		StateTransitionMessage* message;
		UniqueEntityIdentifier source;
		int from;
		int to;
		while (!messages.empty())
		{
			message = messages.front();
			messages.pop();
			// extract info and delete the message
			source = message->source;
			from = message->from;
			to = message->to;
			delete message;
			if (!gs.EntityIsValid(source))
				continue;

			// call the function corresponding to the transition
			sol::function* f = gs.statemachines[source.entity_id].GetTransition(from, to);
			if(f)
				(*f)(&gs, source.entity_id);
			gs.statemachines[source.entity_id].SetState(to);
		}
	}
};