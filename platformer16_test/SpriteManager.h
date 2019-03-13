#pragma once

#include <SDL.h>
#include <unordered_map>
#include "Display.h"
#include "Sprite.h"

class SpriteManager
{
	std::unordered_map<std::string, Sprite*> sprites;
public: 
	SpriteManager();
	~SpriteManager();
	void AddSpriteFromTexture(SDL_Texture* tex, const SDL_Rect rect, int img_count, std::string spr_name);
	Sprite* GetSprite(std::string tex_name);
	void RemoveSprite(std::string tex_name);
};

