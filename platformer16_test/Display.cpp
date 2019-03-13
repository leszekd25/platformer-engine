#include "Display.h"


Display::Display()
{
	window = SDL_CreateWindow("SDL window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


Display::~Display()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}


void Display::SetWindowResolution(int w, int h) const
{
	SDL_SetWindowSize(window, w, h);
}

void Display::SetTitle(const char* title) const
{
	SDL_SetWindowTitle(window, title);
}

SDL_Window* Display::GetWindow() const
{
	return window;
}

SDL_Renderer* Display::GetRenderer() const
{
	return renderer;
}