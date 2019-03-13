#include "GameState.h"

#include <iostream>
#include <string>

GameState::GameState()
{
	entities.resize(MAX_ENTITIES);
	transforms.resize(MAX_ENTITIES);
	movables.resize(MAX_ENTITIES);
	drawables.resize(MAX_ENTITIES);
	animations.resize(MAX_ENTITIES);
	collidables.resize(MAX_ENTITIES);
	damageables.resize(MAX_ENTITIES);
	damagings.resize(MAX_ENTITIES);
	physicss.resize(MAX_ENTITIES);
	inputs.resize(MAX_ENTITIES);
	controls.resize(MAX_ENTITIES);
	statemachines.resize(MAX_ENTITIES);
	timers.resize(MAX_ENTITIES);
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		inputs[i] = InputComponent();
		controls[i] = ControlComponent();
		statemachines[i] = StateMachineComponent();
	}
	active_entities = 0;
	last_active_entity = -1;
	first_free_entity = 0;
	SetFrameRate(60);
	max_entity_id = 0;
	timer.UpdateTime();
	map = 0;
	map_texture = 0;
	map_require_update = false;
	world_properties = WorldProperties();
	world_properties.gravity_strength = 160;
	quit_game = false;
	for (int i = 0; i <= SDL_SCANCODE_APP2; i++)
		keys_pressed[i] = false;
}


GameState::~GameState()
{
	if (map != 0)
		delete map;
	if (map_texture != 0)
		SDL_DestroyTexture(map_texture);
}


void GameState::SetDisplay(Display* d)
{
	display = d;
}

// simply deactivates all entities.
void GameState::ClearAll()
{
	for (int i = 0; i < last_active_entity; i++)
		entities[i].active = false;
	last_active_entity = -1;
	first_free_entity = 0;
	active_entities = 0;
}

// returns first array index which does not contain an active entity
int GameState::GetFirstFreeEntity() const
{
	return first_free_entity;
}

// returns last array index which contains an active entity
// used by systems to determine how far the logic loop should check for entities
int GameState::GetLastActiveEntity() const
{
	return last_active_entity;
}

// essentially creates a new entity, assigning it to a given index
void GameState::ActivateEntity(int i)
{
	if (entities[i].active)
		return;
	entities[i].active = true;
	entities[i].id = max_entity_id;
	for (int j = 0; j < C_MAX; j++)
		entities[i].components[j] = false;

	if (i > last_active_entity)
		last_active_entity = i;
	if (i == first_free_entity)
	{
		for(int j = first_free_entity+1; j < MAX_ENTITIES; j++)
			if (!entities[j].active)
			{
				first_free_entity = j;
				break;
			}
	}

	max_entity_id++;
	active_entities++;
}

// essentially deletes an entity at a given index
void GameState::FreeEntity(int i)
{
	if (!entities[i].active)
		return;
	entities[i].active = false;

	if (i == last_active_entity)
	{
		for(int j = last_active_entity-1; j >= 0; j--)
			if (entities[j].active)
			{
				last_active_entity = j;
				break;
			}
	}
	if (i < first_free_entity)
		first_free_entity = i;

	active_entities--;
}

// determines whether an entity with a given UniqueEntityIdentifier exists
bool GameState::EntityIsValid(const UniqueEntityIdentifier& uid)
{
	if ((uid.entity_id < 0) || (uid.entity_id >= MAX_ENTITIES)) return false;
	return (entities[uid.entity_id].active) && (entities[uid.entity_id].id == uid.gameobject_id);
}

//the four functions below arguments: entity array index, component id (see Components.h)
void GameState::ActivateEntityComponent(int i, int c_id)
{
	entities[i].components[c_id] = true;
}

void GameState::DeactivateEntityComponent(int i, int c_id)
{
	entities[i].components[c_id] = false;
}

void GameState::SwitchEntityComponent(int i, int c_id)
{
	entities[i].components[c_id] ^= true;
}

void GameState::SetEntityComponent(int i, int c_id, bool b)
{
	entities[i].components[c_id] = b;
}

// framerate in frames per second
void GameState::SetFrameRate(int fr)
{
	frames_per_second = fr;
	ticks_per_frame = 1000 / fr;
}

void GameState::SetWindowResolution(int w, int h)
{
	display->SetWindowResolution(w, h);
	view.view_rect.w2 = w / 2; view.view_rect.h2 = h / 2;
	view.tile_dimensions.x = (w / CELL_SIZE) + 2;
	view.tile_dimensions.y = (h / CELL_SIZE) + 2;
}

void GameState::SetViewCenter(Vector pos)
{
	if (pos.x > (map->width*CELL_SIZE) - view.view_rect.w2)
		pos.x = (map->width*CELL_SIZE) - view.view_rect.w2;
	if (pos.x < view.view_rect.w2)
		pos.x = view.view_rect.w2;
	if (pos.y > (map->height*CELL_SIZE) - view.view_rect.h2)
		pos.y = (map->height*CELL_SIZE) - view.view_rect.h2;
	if (pos.y < view.view_rect.h2)
		pos.y = view.view_rect.h2;
	view.view_rect.x = pos.x;
	view.view_rect.y = pos.y;
	view.topleft_cell = Tilepos(int((pos.x-view.view_rect.w2) / CELL_SIZE), int((pos.y-view.view_rect.h2) / CELL_SIZE));
}

void GameState::SetViewEntity(int e_id)
{
	if (e_id < 0)
		view.view_mode = VIEW_FREE;
	else
	{
		view.follow_entity = e_id;
		view.view_mode = VIEW_FOLLOW;
	}
}