/*

Lua bindings for sprite-related utilities
*/

#pragma once

#include <SDL.h>
#include "sol.hpp"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Vector.h"

bool SpriteCreate(SpriteManager* sm, TextureManager* tm, std::string tex_name, Vector pos, Vector size, int img_count, std::string name);

void RegisterLuaSprite(sol::state& lua);