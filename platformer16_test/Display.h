/*

A display class, which holds game window and internal renderer
Renderer is used in many places , see TextureManager, UIElement etc.
*/

#pragma once

#include <SDL.h>
#include <SDL_render.h>


class Display
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
public:
	Display();
	~Display();
	void SetWindowResolution(int w, int h) const;
	void SetTitle(const char* title) const;
	SDL_Window* GetWindow() const;
	SDL_Renderer* GetRenderer() const;
};

