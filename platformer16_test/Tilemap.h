#pragma once

#include <string>
#include "TextureManager.h"
#include "Tileset.h"
#include "Vector.h"
#include "Tilepos.h"

class Tilemap
{
private:
	void GenerateFromString(const std::string& data);
public:
	int width; int height;
	Tileset tile_set;
	int* tile_ids;
	int* collision_masks;

	Tilemap();
	~Tilemap();
	bool Load(const std::string& mapname, TextureManager& tm);
	bool LoadFromPath(const std::string& path, TextureManager& tm);
	void WorldToTile(const Vector& pos, Tilepos& tpos);
	void TileToWorld(const Tilepos& tpos, Vector& pos);
};

