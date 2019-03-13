/*

Lua bindings for map-relateed utilities
*/

#pragma once

#include "sol.hpp"
#include "TextureManager.h"
#include "GameState.h"

bool MapLoad(GameState* gs, TextureManager* tm, const std::string& map_name);
void MapSetTile(GameState* gs, int x, int y, int id);
int MapGetTile(GameState* gs, int x, int y);
void MapSetCollisionMask(GameState* gs, int x, int y, int cmask);
int MapGetCollisionMask(GameState* gs, int x, int y);
Tilepos MapGetSize(GameState* gs);

void RegisterLuaTilemap(sol::state& lua);