#include "LuaTilemap.h"
#include <iostream>

bool MapLoad(GameState* gs, TextureManager* tm, const std::string& map_name)
{
	Tilemap * new_map = new Tilemap();
	if(!new_map->Load(map_name, *tm))
		return false;
	std::cout << "Map loaded..." << std::endl;

	if (gs->map != 0)
		delete gs->map;
	gs->map = new_map;
	gs->map_require_update = true;
	return true;
}

void MapSetTile(GameState* gs, int x, int y, int id)
{
	Tilemap* map = gs->map;
	map->tile_ids[x + y * map->width] = id;
}

int MapGetTile(GameState* gs, int x, int y)
{
	Tilemap* map = gs->map;
	return map->tile_ids[x + y * map->width];
}

void MapSetCollisionMask(GameState* gs, int x, int y, int cmask)
{
	Tilemap* map = gs->map;
	map->collision_masks[x + y * map->width] = cmask;
}

int MapGetCollisionMask(GameState* gs, int x, int y)
{
	Tilemap* map = gs->map;
	return map->collision_masks[x + y * map->width];
}

Tilepos MapGetSize(GameState* gs)
{
	Tilemap* map = gs->map;
	return Tilepos(map->width, map->height);
}

void RegisterLuaTilemap(sol::state& lua)
{
	lua["MapLoad"] = MapLoad;
	lua["MapSetTile"] = MapSetTile;
	lua["MapGetTile"] = MapGetTile;
	lua["MapSetCollisionMask"] = MapSetCollisionMask;
	lua["MapGetCollisionMask"] = MapGetCollisionMask;
	lua["MapGetSize"] = MapGetSize;
}