#include "LuaUIManager.h"

UIElement* UICreateControl()
{
	return new UIElement();
}




void UIAddBaseControl(UIManager* ui, UIElement* elem, const std::string& name, unsigned int priority)
{
	ui->add_container(elem, name, priority);
}

UIElement* UIGetBaseControl(UIManager* ui, const std::string& name)
{
	return ui->get_container(name);
}

UIElement* UIGetControl(UIManager* ui, const std::string& path)
{
	return ui->get_element(path);
}

void UIActivateBaseControl(UIManager* ui, const std::string& name)
{
	ui->activate_container(name);
}

void UIDeactivateBaseControl(UIManager* ui, const std::string& name)
{
	ui->deactivate_container(name);
}



void UIControlAddControl(UIElement* elem_base, UIElement* elem, const std::string& name)
{
	elem_base->add_element(elem, name);
}

void UIControlRemoveControl(UIElement* elem_base, const std::string& name)
{
	elem_base->remove_element(name);
}

UIElement* UIControlGetControl(UIElement* elem_base, const std::string& name)
{
	return elem_base->get_element(name);
}

UIElement* UIControlGetParent(UIElement* elem_base)
{
	return elem_base->parent;
}

std::unordered_map<std::string, UIElement*> UIControlGetChildren(UIElement* elem_base)
{
	return elem_base->children;
}

std::string UIControlGetName(UIElement* elem_base)
{
	return elem_base->name;
}

Rect UIControlGetWorkingArea(UIElement* elem_base)
{
	return elem_base->working_area;
}

std::string UIControlGetText(UIElement* elem_base)
{
	return elem_base->text;
}

bool UIControlGetActive(UIElement* elem_base)
{
	return elem_base->active;
}

bool UIControlGetVisible(UIElement* elem_base)
{
	return elem_base->visible;
}

void UIControlSetWorkingArea(UIElement* elem_base, Rect area)
{
	elem_base->working_area = area;
}

void UIControlSetSprite(UIElement* elem_base, SpriteManager* sm, const std::string& sprite_name)
{
	elem_base->sprite = sm->GetSprite(sprite_name);
}

void UIControlSetText(UIElement* elem_base, const std::string& text)
{
	elem_base->text = text;
}

void UIControlSetTextPosition(UIElement* elem_base, Vector tpos)
{
	elem_base->text_pos = tpos;
}

void UIControlSetActive(UIElement* elem_base, bool active)
{
	if (elem_base->parent != 0)
		elem_base->active = active;
}

void UIControlSetVisible(UIElement* elem_base, bool visible)
{
	if (elem_base->parent != 0)
		elem_base->visible = visible;
}
void UIControlSetMouseTrigger(UIElement* elem_base, bool on_press, bool on_release, int mouse_button)
{
	elem_base->trigger_on_mouse_press = on_press;
	elem_base->trigger_on_mouse_release = on_release;
	elem_base->trigger_mouse_button = mouse_button;
}

void UIControlSetKeyboardTrigger(UIElement* elem_base, bool on_press, int keyboard_scancode)
{
	elem_base->trigger_on_key_press = on_press;
	elem_base->trigger_keyboard_key = keyboard_scancode;
}

void UIControlOnMouseEnterCallback(UIElement* elem_base, sol::function f)
{
	elem_base->on_mouse_enter = new sol::function(f);
}

void UIControlOnMouseLeaveCallback(UIElement* elem_base, sol::function f)
{
	elem_base->on_mouse_leave = new sol::function(f);
}

void UIControlOnMousePressCallback(UIElement* elem_base, sol::function f)
{
	elem_base->on_mouse_press = new sol::function(f);
}

void UIControlOnMouseReleaseCallback(UIElement* elem_base, sol::function f)
{
	elem_base->on_mouse_release = new sol::function(f);
}

void UIControlOnKeyboardPressCallback(UIElement* elem_base, sol::function f)
{
	elem_base->on_key_press = new sol::function(f);
}

void UIControlOnKeyboardReleaseCallback(UIElement* elem_base, sol::function f)
{
	elem_base->on_key_release = new sol::function(f);
}

void UIControlOnTriggerCallback(UIElement* elem_base, sol::function f)
{
	elem_base->on_trigger = new sol::function(f);
}


void RegisterLuaUI(sol::state& lua)
{
	lua["KEY_A"] = SDL_SCANCODE_A;
	lua["KEY_B"] = SDL_SCANCODE_B;
	lua["KEY_C"] = SDL_SCANCODE_C;
	lua["KEY_D"] = SDL_SCANCODE_D;
	lua["KEY_E"] = SDL_SCANCODE_E;
	lua["KEY_F"] = SDL_SCANCODE_F;
	lua["KEY_G"] = SDL_SCANCODE_G;
	lua["KEY_H"] = SDL_SCANCODE_H;
	lua["KEY_I"] = SDL_SCANCODE_I;
	lua["KEY_J"] = SDL_SCANCODE_J;
	lua["KEY_K"] = SDL_SCANCODE_K;
	lua["KEY_L"] = SDL_SCANCODE_L;
	lua["KEY_M"] = SDL_SCANCODE_M;
	lua["KEY_N"] = SDL_SCANCODE_N;
	lua["KEY_O"] = SDL_SCANCODE_O;
	lua["KEY_P"] = SDL_SCANCODE_P;
	lua["KEY_Q"] = SDL_SCANCODE_Q;
	lua["KEY_R"] = SDL_SCANCODE_R;
	lua["KEY_S"] = SDL_SCANCODE_S;
	lua["KEY_T"] = SDL_SCANCODE_T;
	lua["KEY_U"] = SDL_SCANCODE_U;
	lua["KEY_V"] = SDL_SCANCODE_V;
	lua["KEY_W"] = SDL_SCANCODE_W;
	lua["KEY_X"] = SDL_SCANCODE_X;
	lua["KEY_Y"] = SDL_SCANCODE_Y;
	lua["KEY_Z"] = SDL_SCANCODE_Z;

	lua["MOUSE_LEFT"] = SDL_BUTTON_LEFT;
	lua["MOUSE_MIDDLE"] = SDL_BUTTON_MIDDLE;
	lua["MOUSE_RIGHT"] = SDL_BUTTON_RIGHT;

	lua["UICreateControl"] = UICreateControl;

	lua["UIAddBaseControl"] = UIAddBaseControl;
	lua["UIGetBaseControl"] = UIGetBaseControl;
	lua["UIGetControl"] = UIGetControl;
	lua["UIActivateBaseControl"] = UIActivateBaseControl;
	lua["UIDeactivateBaseControl"] = UIDeactivateBaseControl;

	lua["UIControlAddControl"] = UIControlAddControl; 
	lua["UIControlRemoveControl"] = UIControlRemoveControl;
	lua["UIControlGetControl"] = UIControlGetControl;
	lua["UIControlGetParent"] = UIControlGetParent;
	lua["UIControlGetChildren"] = UIControlGetChildren;
	lua["UIControlGetName"] = UIControlGetName;
	lua["UIControlGetWorkingArea"] = UIControlGetWorkingArea;
	lua["UIControlGetText"] = UIControlGetText;
	lua["UIControlGetActive"] = UIControlGetActive;
	lua["UIControlGetVisible"] = UIControlGetVisible;
	lua["UIControlSetWorkingArea"] = UIControlSetWorkingArea;
	lua["UIControlSetSprite"] = UIControlSetSprite;
	lua["UIControlSetText"] = UIControlSetText;
	lua["UIControlSetTextPosition"] = UIControlSetTextPosition;
	lua["UIControlSetActive"] = UIControlSetActive;
	lua["UIControlSetVisible"] = UIControlSetVisible;
	lua["UIControlSetMouseTrigger"] = UIControlSetMouseTrigger;
	lua["UIControlSetKeyboardTrigger"] = UIControlSetKeyboardTrigger;
	lua["UIControlOnMouseEnterCallback"] = UIControlOnMouseEnterCallback;
	lua["UIControlOnMouseLeaveCallback"] = UIControlOnMouseLeaveCallback;
	lua["UIControlOnMousePressCallback"] = UIControlOnMousePressCallback;
	lua["UIControlOnMouseReleaseCallback"] = UIControlOnMouseReleaseCallback;
	lua["UIControlOnKeyboardPressCallback"] = UIControlOnKeyboardPressCallback;
	lua["UIControlOnKeyboardReleaseCallback"] = UIControlOnKeyboardReleaseCallback;
	lua["UIControlOnTriggerCallback"] = UIControlOnTriggerCallback;
}