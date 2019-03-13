/*

GameLogic owns game state and holds game loop. Important stuff.
It also holds all systems used in the game.
*/

#pragma once

#include "System.h"
#include "GameState.h"

class GameLogic
{
public:
	GameState game_state;
	// systems here
	InputSystem input_system;
	MovementSystem movement_system;
	AnimationSystem animation_system;
	RenderingSystem rendering_system;
	CollisionDetectionSystem collision_detection_system;
	CollisionResolveQuasiSystem collision_resolve_quasi_system;
	DamageResolveQuasiSystem damage_resolve_quasi_system;
	StateMachineSystem state_machine_system;
	// systems here

	GameLogic();
	~GameLogic();

	void ProcessMessages();
	int Step();
};

