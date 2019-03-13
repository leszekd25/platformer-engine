/*

Lua bindings for UI manipulation functions
*/


#pragma once

#include <SDL.h>
#include "sol.hpp"
#include "UIManager.h"
#include "SpriteManager.h"

UIElement* UICreateControl();

void UIAddBaseControl(UIManager* ui, UIElement* elem, const std::string& name, unsigned int priority);
UIElement* UIGetBaseControl(UIManager* ui, const std::string& name);
UIElement* UIGetControl(UIManager* ui, const std::string& path);
void UIActivateBaseControl(UIManager* ui, const std::string& name);
void UIDeactivateBaseControl(UIManager* ui, const std::string& name);

void UIControlAddControl(UIElement* elem_base, UIElement* elem, const std::string& name);
void UIControlRemoveControl(UIElement* elem_base, const std::string& name);
UIElement* UIControlGetControl(UIElement* elem_base, const std::string& name);
UIElement* UIControlGetParent(UIElement* elem_base);
std::unordered_map<std::string, UIElement*>  UIControlGetChildren(UIElement* elem_base);
std::string UIControlGetName(UIElement* elem_base);
Rect UIControlGetWorkingArea(UIElement* elem_base);
std::string UIControlGetText(UIElement* elem_base);
bool UIControlGetActive(UIElement* elem_base);
bool UIControlGetVisible(UIElement* elem_base);
void UIControlSetWorkingArea(UIElement* elem_base, Rect area);
void UIControlSetSprite(UIElement* elem_base, SpriteManager* sm, const std::string& sprite_name);
void UIControlSetText(UIElement* elem_base, const std::string& text);
void UIControlSetTextPosition(UIElement* elem_base, Vector tpos);
void UIControlSetActive(UIElement* elem_base, bool active);
void UIControlSetVisible(UIElement* elem_base, bool visible);
void UIControlSetMouseTrigger(UIElement* elem_base, bool on_press, bool on_release, int mouse_button);
void UIControlSetKeyboardTrigger(UIElement* elem_base, bool on_press, int keyboard_scancode);
void UIControlOnMouseEnterCallback(UIElement* elem_base, sol::function f);
void UIControlOnMouseLeaveCallback(UIElement* elem_base, sol::function f);
void UIControlOnMousePressCallback(UIElement* elem_base, sol::function f);
void UIControlOnMouseReleaseCallback(UIElement* elem_base, sol::function f);
void UIControlOnKeyboardPressCallback(UIElement* elem_base, sol::function f);
void UIControlOnKeyboardReleaseCallback(UIElement* elem_base, sol::function f);
void UIControlOnTriggerCallback(UIElement* elem_base, sol::function f);

void RegisterLuaUI(sol::state& lua);