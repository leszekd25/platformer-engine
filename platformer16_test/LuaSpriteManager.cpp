#include "LuaSpriteManager.h"

bool SpriteCreate(SpriteManager* sm, TextureManager* tm, std::string tex_name, Vector pos, Vector size, int img_count, std::string name)
{
	SDL_Rect rect;
	rect.x = pos.x; rect.y = pos.y;
	rect.w = size.x; rect.h = size.y;
	SDL_Texture* tx = tm->GetTexture(tex_name);
	if (tx == 0)
		return false;
	sm->AddSpriteFromTexture(tm->GetTexture(tex_name), rect, img_count, name);
	return true;
}

void RegisterLuaSprite(sol::state& lua)
{
	lua["SpriteCreate"] = SpriteCreate;
}