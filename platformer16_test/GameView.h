#pragma once

#include "Tilepos.h"
#include "Rect.h"

enum ViewMode {VIEW_FREE = 0, VIEW_FOLLOW};

struct GameView
{
	Rect view_rect;
	Tilepos topleft_cell;
	Tilepos tile_dimensions;
	ViewMode view_mode;
	int follow_entity;
};