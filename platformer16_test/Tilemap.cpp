#include "Tilemap.h"

#include <cassert>
#include <filesystem>
#include <iostream>
#include "pugixml.hpp"


// takes data from map file generated by tiled map editor
void ParseNextNumber(const std::string& data, unsigned int& pos, int& result)
{
	if (pos >= data.length())
	{
		pos = -1;
		return;
	}
	result = 0;
	while ((data[pos] != ','))   // why is 10 here... it is in data, but why? (hint: csv encoding?)
	{
		if (data[pos] == 10)
		{
			pos++;
			if (pos == data.length())
				break;
			continue;
		}
		result *= 10;
		result += data[pos] - 48;
		pos++;
	}
	if (pos == data.length())
		return;
	pos++;
	if (data[pos] == '\n')
		pos++;
}

Tilemap::Tilemap()
{
	width = 0; height = 0;
	tile_ids = 0;
	collision_masks = 0;
	tile_set = Tileset();
}


Tilemap::~Tilemap()
{
	if (tile_ids != 0)
	{
		delete[] tile_ids;
		delete[] collision_masks;
	}
}

void Tilemap::GenerateFromString(const std::string& data)
{
	unsigned int pos = 0;
	int tile_id = 0;
	for(int i = 0; i < width*height; i++)
	{
		ParseNextNumber(data, pos, tile_id);
		assert(pos != -1);
		tile_id--;
		tile_ids[i] = tile_id;
		collision_masks[i] = (tile_id != -1?1:0);
	}
}

bool Tilemap::Load(const std::string& ts_name, TextureManager& tm)
{
	std::string path = "maps\\" + ts_name + ".tmx";
	return Tilemap::LoadFromPath(path, tm);
}

// loads map file from tiled map editor
bool Tilemap::LoadFromPath(const std::string& path, TextureManager& tm)
{
	if (!std::experimental::filesystem::exists(path))
		return false;

	// load file to xml parser
	pugi::xml_document ts;
	pugi::xml_parse_result res = ts.load_file(path.c_str());
	if (!res)
		return false;

	// get tileset data
	pugi::xml_node ts_node = ts.child("map");
	width = ts_node.attribute("width").as_int();
	height = ts_node.attribute("height").as_int();
	tile_ids = new int[width*height];
	collision_masks = new int[width*height];

	pugi::xml_node ts_node2 = ts_node.child("tileset");
	std::string tileset_path = ts_node2.attribute("source").as_string();
	tileset_path = tileset_path.erase(0, 3);    // remove ../ from path
	if (!tile_set.LoadFromPath(tileset_path, tm))
		return false;

	// get collision tile layer data
	for (pugi::xml_node layer_node = ts_node.child("layer"); layer_node; layer_node = layer_node.next_sibling("layer"))
	{
		if (std::string(layer_node.attribute("name").as_string()) == "tiles_collision")
		{
			std::string tile_data = layer_node.child_value("data");
			// process tile data
			GenerateFromString(tile_data);
			return true;
		}
	}
	return false;
}

void Tilemap::WorldToTile(const Vector& pos, Tilepos& tpos)
{
	tpos.x = ((int)pos.x) / CELL_SIZE;
	tpos.y = ((int)pos.y) / CELL_SIZE;
}

void Tilemap::TileToWorld(const Tilepos& tpos, Vector& pos)
{
	pos.x = tpos.x * CELL_SIZE;
	pos.y = tpos.y * CELL_SIZE;
}