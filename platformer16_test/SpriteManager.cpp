#include "SpriteManager.h"

SpriteManager::SpriteManager()
{
	sprites = std::unordered_map<std::string, Sprite*>();
}


SpriteManager::~SpriteManager()
{
	for (auto it = sprites.begin(); it != sprites.end(); ++it)
		delete it->second;
}

void SpriteManager::AddSpriteFromTexture(SDL_Texture* tex, const SDL_Rect rect, int img_count, std::string spr_name)
{
	sprites.emplace(spr_name, new Sprite(tex, rect, img_count, spr_name));
}

Sprite* SpriteManager::GetSprite(std::string tex_name)
{
	return sprites[tex_name];
}

void SpriteManager::RemoveSprite(std::string tex_name)
{
	auto it = sprites.find(tex_name);
	if (it != sprites.end())
		sprites.erase(it);
}