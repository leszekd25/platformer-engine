#pragma once

#include <string>
#include <unordered_map>
#include <SDL.h>
#include "SpriteManager.h"
#include "Vector.h"
#include "Rect.h"
#include "sol.hpp"

class UIElement
{
public:
	UIElement* parent;
	std::unordered_map<std::string, UIElement*> children;
	std::string name;
	Rect working_area;
	Sprite* sprite;
	std::string text;
	Vector text_pos;
	bool active;
	bool visible;
	bool focused;
	bool mouseover;
	bool trigger_on_mouse_press;
	bool trigger_on_mouse_release;
	bool trigger_on_key_press;
	int trigger_mouse_button;
	int trigger_keyboard_key;
	sol::function* on_active_changed;               
	sol::function* on_visible_changed;   
	sol::function* on_focused_changed;                  
	sol::function* on_mouse_enter;
	sol::function* on_mouse_leave;
	sol::function* on_mouse_press;     // mouse_key
	sol::function* on_mouse_release;   // mouse_key
	sol::function* on_key_press;       // keyboard_key
	sol::function* on_key_release;     // keyboard_key
	sol::function* on_trigger;
	UIElement() {
		parent = 0; children = std::unordered_map<std::string, UIElement*>(); sprite = 0; text = "";
		on_active_changed = 0; on_visible_changed = 0; on_focused_changed = 0;
		on_mouse_press = 0; on_mouse_enter = 0; on_mouse_leave = 0; on_mouse_release = 0;
		on_key_press = 0; on_key_release = 0; on_trigger = 0;
		active = true; visible = true;
	}
	~UIElement() {
		if (on_active_changed != 0) delete on_active_changed;
		if (on_visible_changed != 0) delete on_visible_changed;
		if (on_focused_changed != 0) delete on_focused_changed;
		if (on_mouse_press != 0) delete on_mouse_press;
		if (on_mouse_enter != 0) delete on_mouse_enter;
		if (on_mouse_leave != 0) delete on_mouse_leave;
		if (on_mouse_release != 0) delete on_mouse_release;
		if (on_key_press != 0) delete on_key_press;
		if (on_key_release != 0) delete on_key_release;
		if (on_trigger != 0) delete on_trigger;
		for (auto it = children.begin(); it != children.end(); ++it)
			delete it->second;
	};

	void add_element(UIElement* elem, const std::string& _name);
	void remove_element(const std::string& _name);
	UIElement* get_element(const std::string& _name);
	void propagate_event(SDL_Event* ev);
	void render(SDL_Renderer* renderer);
};