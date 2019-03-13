#include "Tileset.h"

#include <filesystem>
#include <iostream>
#include "pugixml.hpp"

Tileset::Tileset()
{
	tile_count = 0;
	column_count = 0;
	texture = 0;
	tile_rects = 0;
}

Tileset::~Tileset()
{
	if (tile_rects != 0)
		delete[] tile_rects;
}

bool Tileset::Load(const std::string& ts_name, TextureManager& tm)
{
	std::string path = "tilesets\\" + ts_name + ".tsx";
	return Tileset::LoadFromPath(path, tm);
}

bool Tileset::LoadFromPath(const std::string& path, TextureManager& tm)
{
	if (!std::experimental::filesystem::exists(path))
		return false;

	// load file to xml parser
	pugi::xml_document ts;
	pugi::xml_parse_result res = ts.load_file(path.c_str());
	if (!res)
		return false;

	// get tileset data
	pugi::xml_node ts_node = ts.child("tileset");
	int tile_width = ts_node.attribute("tilewidth").as_int();
	int tile_height = ts_node.attribute("tileheight").as_int();
	tile_count = ts_node.attribute("tilecount").as_int();
	column_count = ts_node.attribute("columns").as_int();

	ts_node = ts_node.child("image");
	std::string texture_path = ts_node.attribute("source").as_string();
	texture_path = texture_path.erase(0, 3);    // remove ../ from path

	// process data 
	std::string texture_name = std::experimental::filesystem::path(texture_path).replace_extension("").filename().generic_string();
	texture = tm.GetTexture(texture_name);
	if (texture == 0)
	{
		if(!tm.AddTextureFromFile(texture_path, texture_name))
			return false;
		texture = tm.GetTexture(texture_name);
	}

	tile_rects = new SDL_Rect[tile_count];
	for (int i = 0; i < tile_count; i++)
	{
		tile_rects[i].x = (i % column_count) * tile_width;
		tile_rects[i].y = (i / column_count) * tile_height;
		tile_rects[i].w = tile_width;
		tile_rects[i].h = tile_height;
	}

	return true;
}