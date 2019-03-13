#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include "UIElement.h"

class UIManager
{
	void calculate_max_priority()
	{
		max_priority = 0;
		for (int i = 0; i < priority.size(); i++)
		{
			if (!containers[i]->active)
				continue;
			if (priority[i] > max_priority)
				max_priority = priority[i];
		}
	}

	void sort_priority()
	{
		if (container_priority.size() != containers.size())
		{
			container_priority.clear();
			for (int i = 0; i < containers.size(); i++)
				container_priority.push_back(i);
		}

		// bubble sort is enough for this use... probably...
		for (int i = 0; i < containers.size(); i++)
		{
			for (int j = i; j < containers.size()-1; j++)
			{
				if (priority[container_priority[i]] > priority[container_priority[i+1]])
				{
					unsigned int k = container_priority[i];
					container_priority[i] = container_priority[j];
					container_priority[j] = k;
				}
			}
		}
	}
public:
	std::vector<UIElement*> containers;            // 0 1 2 3 4 5
	std::vector<unsigned int> priority;            // 0 2 0 3 1 2
	std::vector<unsigned int> container_priority;  // 0 2 4 1 5 3
	unsigned int max_priority;
	UIManager()
	{
		containers = std::vector<UIElement*>();
		priority = std::vector<unsigned int>();
		container_priority = std::vector<unsigned int>();
		max_priority = 0;
	}
	~UIManager()
	{
		for (auto it = containers.begin(); it != containers.end(); ++it)
			delete *it;
	}
	void add_container(UIElement* cont, std::string name, unsigned int p);
	void remove_container(std::string name);
	UIElement* get_container(std::string name);
	void activate_container(std::string name);
	void deactivate_container(std::string name);
	UIElement* get_element(std::string path);
	void resolve_event(SDL_Event* ev);
	void render(SDL_Renderer* renderer);
};

