#include "UIElement.h"

void UIElement::add_element(UIElement* elem, const std::string& _name)
{
	children.emplace(_name, elem);
	elem->parent = this;
	elem->name = _name;
}

void UIElement::remove_element(const std::string& _name)
{
	auto it = children.find(_name);
	if (it != children.end())
	{
		delete it->second;
		children.erase(it);
	}
}

UIElement* UIElement::get_element(const std::string& _name)
{
	auto it = children.find(_name);
	if (it == children.end())
		return 0;
	return it->second;
}

void UIElement::propagate_event(SDL_Event* ev)
{
	if (!active)
		return;
	bool propagate = true;
	switch (ev->type)
	{
		case SDL_KEYDOWN:
		{
			if ((trigger_on_key_press) && (trigger_keyboard_key == ev->key.keysym.scancode))
				if (on_trigger)
					(*on_trigger)(this);
			if (on_key_press)
				(*on_key_press)(this, ev->key.keysym.scancode);
			break;
		}
		case SDL_KEYUP:
		{
			if (on_key_release)
				(*on_key_release)(this, ev->key.keysym.scancode);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			if (mouseover)
			{
				if ((trigger_on_mouse_press) && (trigger_mouse_button == ev->button.button))
					if (on_trigger)
						(*on_trigger)(this);
				if (on_mouse_press)
					(*on_mouse_press)(this, ev->button.button);
			}
			else
				propagate = false;
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if (mouseover)
			{
				if ((trigger_on_mouse_release) && (trigger_mouse_button == ev->button.button))
					if (on_trigger)
						(*on_trigger)(this);
				if (on_mouse_release)
					(*on_mouse_release)(this, ev->button.button);
			}
			else
				propagate = false;
			break;
		}
		case SDL_MOUSEMOTION:
		{
			if (mouseover)
			{
				if (!(working_area.Contains(Vector(ev->button.x, ev->button.y))))
				{
					mouseover = false;
					if (on_mouse_leave)
						(*on_mouse_leave)(this);
				}
				else
					propagate = false;
			}
			else
			{
				if (working_area.Contains(Vector(ev->button.x, ev->button.y)))
				{
					mouseover = true;
					if (on_mouse_enter)
						(*on_mouse_enter)(this);
				}
				else
					propagate = false;
			}
			break;
		}
	}


	if (propagate)
	{
		for (auto it = children.begin(); it != children.end(); ++it)
			it->second->propagate_event(ev);
	}
}

void UIElement::render(SDL_Renderer* renderer)
{
	if (!visible)
		return;

	SDL_Rect screen_area;
	screen_area.x = working_area.x - working_area.w2;
	screen_area.y = working_area.y - working_area.h2;
	screen_area.w = 2 * working_area.w2;
	screen_area.h = 2 * working_area.h2;
	if (parent)
	{
		screen_area.x += parent->working_area.x - parent->working_area.w2;
		screen_area.y += parent->working_area.y - parent->working_area.h2;
	}

	if (sprite)
		SDL_RenderCopy(renderer, sprite->texture, &(sprite->rect), &screen_area);

	for (auto it = children.begin(); it != children.end(); ++it)
		it->second->render(renderer);

}