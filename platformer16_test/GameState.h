/*

Most important part of the game: the game state.
Game state holds every internal game-related variable in the application

Note on entities:
While each entity is contained in one and exactly one entry of entity array,
this does not mean that this entity can't change over time. Entities can be
destroyed and created on the fly, so to avoid entity mismatching,
UniqueEntityIdentifier is used to determine with 100% certainty if a requested entity exists.
See GameObject.h
Each entity is created with an unique ID number, which might or might not match its
array index in entity array. The ID number will never repeat over the course of the game.

TODO: safety checks to all methods...
*/


#pragma once

#include <vector>
#include <queue>
#include "sol.hpp"
#include "Component.h"
#include "GameObject.h"
#include "Time.h"
#include "Display.h"
#include "UIManager.h"
#include "Message.h"
#include "Tilemap.h"
#include "WorldProperties.h"
#include "GameView.h"

class GameState
{
public:
	Display* display;         // does not own this display!
	UIManager* ui_manager;    // does not own this manager!
	sol::state* lua;          // does not own this lua state!

	Time timer;               // owns this timer!
	int frames_per_second;
	int ticks_per_frame;
	int active_entities;
	int last_active_entity;
	int first_free_entity;
	int max_entity_id;
	WorldProperties world_properties;  // owns these properties!
	std::vector<GameObject> entities;
	std::vector<TransformComponent> transforms;
	std::vector<MovableComponent> movables;
	std::vector<DrawableComponent> drawables;
	std::vector<AnimationComponent> animations;
	std::vector<CollidableComponent> collidables;
	std::vector<DamageableComponent> damageables;
	std::vector<DamagingComponent> damagings;
	std::vector<PhysicsComponent> physicss;
	std::vector<InputComponent> inputs;
	std::vector<ControlComponent> controls;
	std::vector<StateMachineComponent> statemachines;
	std::vector<TimerComponent> timers;
	std::queue<Message*> messages;    // owns this queue!
	bool keys_pressed[SDL_SCANCODE_APP2+1];    // an array which holds state of all keyboard keys
	Tilemap* map;                     // owns this tilemap!
	GameView view;
	SDL_Texture* map_texture;         //owns this map texture!
	bool map_require_update;
	bool quit_game;

	GameState();
	~GameState();
	void SetDisplay(Display* d);
	void ClearAll();
	int GetFirstFreeEntity() const;
	int GetLastActiveEntity() const;
	void ActivateEntity(int id);
	void FreeEntity(int id);
	bool EntityIsValid(const UniqueEntityIdentifier& uid);
	void ActivateEntityComponent(int e_id, int c_id);
	void DeactivateEntityComponent(int e_id, int c_id);
	void SwitchEntityComponent(int e_id, int c_id);
	void SetEntityComponent(int e_id, int c_id, bool set);
	void SetFrameRate(int fr);
	void SetWindowResolution(int w, int h);
	void SetViewCenter(Vector pos);
	void SetViewEntity(int e_id);
};

