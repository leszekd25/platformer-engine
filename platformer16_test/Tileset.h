#pragma once

#include <string>
#include <SDL.h>
#include "TextureManager.h"

class Tileset
{
	int tile_count;
	int column_count;
public:
	SDL_Texture* texture;
	SDL_Rect* tile_rects;

	Tileset();
	~Tileset();
	bool Load(const std::string& ts_name, TextureManager& tm);
	bool LoadFromPath(const std::string& path, TextureManager& tm);
};