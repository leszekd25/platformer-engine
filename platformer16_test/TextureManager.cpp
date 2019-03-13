#include "TextureManager.h"

#include <SDL_image.h>

#include <iostream>
#include <string>


TextureManager::TextureManager()
{
	textures = std::unordered_map<std::string, SDL_Texture*>();
}


TextureManager::~TextureManager()
{
	for (auto it = textures.begin(); it != textures.end(); ++it)
		SDL_DestroyTexture(it->second);
}

void TextureManager::SetDisplay(Display* d)
{
	display = d;
}

bool TextureManager::AddTextureFromFile(std::string image_path, std::string tex_name)
{         
	SDL_Surface* surf = IMG_Load(image_path.c_str());
	if (surf == 0)
	{
		std::cout << "Failed to load image " << image_path << std::endl;
		return false;
	}
	textures.emplace(tex_name, SDL_CreateTextureFromSurface(display->GetRenderer(), surf));
	SDL_FreeSurface(surf);              
	return true;
}

void TextureManager::AddTextureFromSurface(SDL_Surface* surf, std::string tex_name)
{
	textures.emplace(tex_name, SDL_CreateTextureFromSurface(display->GetRenderer(), surf));
}

void TextureManager::AddEmptyTexture(int w, int h, Color col, std::string tex_name)
{
	SDL_Texture* tex = SDL_CreateTexture(display->GetRenderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);
	Color cur;
	SDL_GetRenderDrawColor(display->GetRenderer(), &(cur.r), &(cur.g), &(cur.b), &(cur.a));
	SDL_SetRenderTarget(display->GetRenderer(), tex);
	SDL_SetRenderDrawColor(display->GetRenderer(), col.r, col.g, col.b, col.a);
	SDL_RenderClear(display->GetRenderer());
	SDL_SetRenderDrawColor(display->GetRenderer(), cur.r, cur.g, cur.b, cur.a);
	SDL_SetRenderTarget(display->GetRenderer(), 0);
	textures.emplace(tex_name, tex);
} 

SDL_Texture* TextureManager::GetTexture(std::string tex_name)
{
	auto it = textures.find(tex_name);
	if (it == textures.end())
		return 0;
	return it->second;
}

void TextureManager::FreeTexture(std::string tex_name)
{
	auto it = textures.find(tex_name);
	if (it != textures.end())
	{
		SDL_DestroyTexture(it->second);
		textures.erase(it);
	}
}