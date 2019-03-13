#pragma once

#include <SDL.h>
#include <unordered_map>
#include "Display.h"
#include "Color.h"


class TextureManager
{
	std::unordered_map<std::string, SDL_Texture*> textures;
	Display* display;
public:
	TextureManager();
	~TextureManager();
	void SetDisplay(Display* d);
	bool AddTextureFromFile(std::string image_path, std::string tex_name);
	void AddTextureFromSurface(SDL_Surface* surf, std::string tex_name);
	void AddEmptyTexture(int w, int h, Color col, std::string tex_name);
	SDL_Texture* GetTexture(std::string tex_name);
	void FreeTexture(std::string tex_name);
};

