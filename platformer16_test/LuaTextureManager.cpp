#include "LuaTextureManager.h"

bool TextureLoad(TextureManager* tm, std::string filepath, std::string name)
{
	return tm->AddTextureFromFile(filepath, name);
}

void RegisterLuaTexture(sol::state& lua)
{
	lua["TextureLoad"] = TextureLoad;
}