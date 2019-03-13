/*

Lua bindings for texture-related utilities
*/

#pragma once

#include <SDL.h>
#include "sol.hpp"
#include "TextureManager.h"

bool TextureLoad(TextureManager* tm, std::string filepath, std::string name);

void RegisterLuaTexture(sol::state& lua);