#include <iostream>

#include <SDL.h>
#include "sol.hpp"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "UIManager.h"
#include "GameLogic.h"

#include "LuaSpriteManager.h"
#include "LuaTextureManager.h"
#include "LuaGameState.h"
#include "LuaTilemap.h"
#include "LuaUIManager.h"

sol::state lua;
Display* display;
UIManager* ui_manager;
SpriteManager* sprite_manager;
TextureManager* texture_manager;
GameLogic* game_logic;

TextureManager* GetTextureManager()
{
	return texture_manager;
}

SpriteManager* GetSpriteManager()
{
	return sprite_manager;
}

UIManager* GetUIManager()
{
	return ui_manager;
}

GameState* GetGameState()
{
	return &(game_logic->game_state);
}

void RegisterLuaGlobalFunctions(sol::state& lua)
{
	lua["GetTextureManager"] = GetTextureManager;
	lua["GetSpriteManager"] = GetSpriteManager;
	lua["GetUIManager"] = GetUIManager;
	lua["GetGameState"] = GetGameState;
}

void RegisterLuaUserTypes(sol::state& lua)
{
	lua.new_usertype<Color>("Color",
		sol::constructors<Color(), Color(unsigned char, unsigned char, unsigned char), Color(unsigned char, unsigned char, unsigned char, unsigned char)>(),
		"red", &Color::r,
		"green", &Color::g,
		"blue", &Color::b,
		"alpha", &Color::a);
	lua.new_usertype<Vector>("Vector",
		sol::constructors<Vector(), Vector(float, float)>(),
		"x", &Vector::x,
		"y", &Vector::y);
	lua.new_usertype<Rect>("Rect",
		sol::constructors<Rect(), Rect(float, float, float, float), Rect(Vector, Vector)>(),
		"x", &Rect::x,
		"y", &Rect::y,
		"w2", &Rect::w2,
		"h2", &Rect::h2);
	lua.new_usertype<Tilepos>("Tilepos",
		sol::constructors<Tilepos(), Tilepos(int, int)>(),
		"x", &Tilepos::x,
		"y", &Tilepos::y);
}

sol::protected_function_result sol_lua_error(lua_State* state, sol::protected_function_result pfr) {
	// pfr will contain things that went wrong, for either loading or executing the script
	// the user can do whatever they like here, including throw. Otherwise...
	sol::error err = pfr;
	std::cout << "An error (an expected one) occurred: " << err.what() << std::endl;

	// ... they need to return the protected_function_result
	return pfr;
}

int main(int argc, char *argv[])
{
	// system initialization

	lua.open_libraries(sol::lib::base);
	lua.open_libraries(sol::lib::math);

	RegisterLuaUserTypes(lua);
	RegisterLuaGlobalFunctions(lua);
	RegisterLuaTexture(lua);
	RegisterLuaSprite(lua);
	RegisterLuaGameState(lua);
	RegisterLuaTilemap(lua);
	RegisterLuaUI(lua);

	SDL_Init(SDL_INIT_VIDEO);

	display = new Display();
	texture_manager = new TextureManager();
	sprite_manager = new SpriteManager();
	ui_manager = new UIManager();
	texture_manager->SetDisplay(display);
	game_logic = new GameLogic();
	game_logic->game_state.SetDisplay(display);
	game_logic->game_state.ui_manager = ui_manager;
	game_logic->game_state.lua = &lua;

	game_logic->game_state.SetWindowResolution(640, 480);

	// game initialization scripts

	auto result = lua.safe_script_file("scripts\\init.lua", sol_lua_error);

	game_logic->game_state.SetViewCenter(Vector(300, 240));
	game_logic->game_state.SetViewEntity(0);

	game_logic->game_state.timer.UpdateTime();
	// game loop

	while (game_logic->Step() == 0);

	// cleanup

	delete ui_manager;
	delete game_logic;
	delete sprite_manager;
	delete texture_manager;
	delete display;
	SDL_Quit();
	
	return 0;
}