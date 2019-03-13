/*

Rendering system draws everything - tilemap, entities, and UI
It is the last system activated each frame
*/


#pragma once

#include <vector>
#include <SDL.h>
#include <iostream>
#include "Display.h"
#include "Component.h"
#include "GameObject.h"
#include "GameState.h"

class RenderingSystem
{
public:
	RenderingSystem() {  };
	~RenderingSystem() {  };

	// Creates a texture on which the map is drawn
	void GenerateMapSurface(GameState& gs)
	{
		SDL_Renderer* renderer = gs.display->GetRenderer();
		if (gs.map_texture != 0)
			SDL_DestroyTexture(gs.map_texture);
		gs.map_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET,
			gs.view.tile_dimensions.x * CELL_SIZE, gs.view.tile_dimensions.y * CELL_SIZE);
		UpdateMapSurface(renderer, gs.map_texture, gs.map, gs.view);
	}

	// Draws map on the map texture
	// TODO: make it work with moving view
	void UpdateMapSurface(SDL_Renderer* renderer, SDL_Texture* map_texture, Tilemap* map, GameView& view)
	{
		std::cout << SDL_SetRenderTarget(renderer, map_texture) << std::endl;

		Color cur;
		SDL_GetRenderDrawColor(renderer, &(cur.r), &(cur.g), &(cur.b), &(cur.a));
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, cur.r, cur.g, cur.b, cur.a);

		Tileset& ts = map->tile_set;
		SDL_Rect dst_rect;
		dst_rect.w = CELL_SIZE;
		dst_rect.h = CELL_SIZE;
		int tile_id;

		int start_x, start_y, bound_x, bound_y, dim_x, dim_y;
		start_x = view.topleft_cell.x;
		bound_x = start_x + view.tile_dimensions.x;
		if (bound_x > map->width)
			bound_x = map->width;
		dim_x = bound_x - start_x;
		start_y = view.topleft_cell.y;
		bound_y = start_y + view.tile_dimensions.y;
		if (bound_y > map->height)
			bound_y = map->height;
		dim_y = bound_y - start_y;


		for (int i = 0; i < dim_y; i++)
		{
			dst_rect.y = i * CELL_SIZE;
			for (int j = 0; j < dim_x; j++)
			{
				dst_rect.x = j * CELL_SIZE;
				tile_id = map->tile_ids[(start_y + i)*map->width + start_x + j];
				if(tile_id >= 0)
					SDL_RenderCopy(renderer, ts.texture, &(ts.tile_rects[tile_id]), &dst_rect);
			}
		}

		SDL_SetRenderTarget(renderer, 0);
	}

	// renders map texture to the screen
	void RenderMapTexture(SDL_Renderer* renderer, SDL_Texture* map_texture, Tilemap* map, GameView& view)
	{
		SDL_Rect map_rect;
		SDL_Rect dst_rect;
		map_rect.x = 0;
		map_rect.y = 0;
		map_rect.w = view.tile_dimensions.x * CELL_SIZE;
		map_rect.h = view.tile_dimensions.y * CELL_SIZE;
		//SDL_RenderGetViewport(renderer, &dst_rect);
		dst_rect.x = (view.topleft_cell.x*CELL_SIZE) - (view.view_rect.x - view.view_rect.w2);
		dst_rect.y = (view.topleft_cell.y*CELL_SIZE) - (view.view_rect.y - view.view_rect.h2);
		dst_rect.w = view.tile_dimensions.x * CELL_SIZE;
		dst_rect.h = view.tile_dimensions.y * CELL_SIZE;

		SDL_RenderCopy(renderer, map_texture, &map_rect, &dst_rect);
	}

	// main render function
	void Step(GameState& gs)
	{
		std::vector<GameObject>& entities = gs.entities;
		std::vector<TransformComponent>& transforms = gs.transforms;
		std::vector<DrawableComponent>& drawables = gs.drawables;
		std::vector<AnimationComponent>& animations = gs.animations;
		Display* display = gs.display;
		GameView& view = gs.view;
		int last_active_entity = gs.last_active_entity;

		int image_index;
		SDL_Rect source;
		SDL_Rect target;
		SDL_Renderer* renderer = display->GetRenderer();

		SDL_RenderClear(renderer);

		// move view
		Tilepos current_tile = view.topleft_cell;
		if (view.view_mode == VIEW_FOLLOW)
			gs.SetViewCenter(transforms[view.follow_entity].position);
		if (current_tile != view.topleft_cell)
			gs.map_require_update = true;

		// draw map
		if (gs.map)
		{
			if (gs.map_require_update)
			{
				if (!gs.map_texture)
					GenerateMapSurface(gs);
				UpdateMapSurface(renderer, gs.map_texture, gs.map, gs.view);
				gs.map_require_update = false;
			}
			RenderMapTexture(renderer, gs.map_texture, gs.map, gs.view);
		}

		// draw entities
		for (int i = 0; i <= last_active_entity; i++)
		{
			if (!entities[i].active)
				continue;
			image_index = 0;
			if ((entities[i].components[C_TRANSFORM]) && (entities[i].components[C_DRAWABLE]))
			{
				if (entities[i].components[C_ANIMATION])
					image_index = animations[i].current_image;
				if (drawables[i].sprite != 0)
				{
					source.x = drawables[i].sprite->rect.x + image_index * drawables[i].sprite->image_width;
					source.y = drawables[i].sprite->rect.y;
					source.w = drawables[i].sprite->image_width;
					source.h = drawables[i].sprite->image_height;
					target.x = transforms[i].position.x - view.view_rect.x + view.view_rect.w2;
					target.y = transforms[i].position.y - view.view_rect.y + view.view_rect.h2;
					target.w = source.w;
					target.h = source.h;
					SDL_RenderCopy(renderer, drawables[i].sprite->texture, &source, &target);
				}
			}
		}

		// draw ui
		gs.ui_manager->render(renderer);

		SDL_RenderPresent(renderer);
	}
};