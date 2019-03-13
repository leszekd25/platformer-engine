/*

Sprite is a data block which holds information about a single graphics piece, which may consist of several images
*/

#pragma once

#include <SDL.h>

struct Sprite
{
	SDL_Texture* texture;
	SDL_Rect rect;
	int image_count;
	int image_width;
	int image_height;
	std::string name;
	Sprite(SDL_Texture* tex, const SDL_Rect r, int imgc, std::string n) : texture(tex), rect(r), image_count(imgc), name(n)
	{
		image_width = rect.w / image_count;
		image_height = rect.h;
	}
};