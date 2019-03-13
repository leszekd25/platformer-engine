#include "UIManager.h"

#include <cassert>

void UIManager::add_container(UIElement* cont, std::string name, unsigned int p)
{
	containers.push_back(cont);
	cont->name = name;
	cont->active = false;
	cont->visible = false;
	priority.push_back(p);
	sort_priority();
}

void UIManager::remove_container(std::string name)
{
	int elem_index = -1;
	for (int i = 0; i < containers.size(); i++)
	{
		if (containers[i]->name == name)
		{
			elem_index = i;
			assert(!containers[i]->active);
			break;
		}
	}
	if (elem_index == -1)
		return;

	delete containers[elem_index];
	containers.erase(containers.begin() + elem_index);
	int p = priority[elem_index];
	priority.erase(priority.begin() + elem_index);

	if (p >= max_priority)
		calculate_max_priority();

	sort_priority();
}

UIElement* UIManager::get_container(std::string name)
{
	for (int i = 0; i < containers.size(); i++)
	{
		if (containers[i]->name == name)
		{
			return containers[i];
		}
	}
	return 0;
}

void UIManager::activate_container(std::string name)
{
	int cont_index = -1;
	UIElement* cont = 0;
	for (int i = 0; i < containers.size(); i++)
	{
		if (containers[i]->name == name)
		{
			cont = containers[i];
			cont_index = i;
		}
	}
	if (cont == 0)
		return;

	cont->active = true;
	cont->visible = true;

	if (priority[cont_index] > max_priority)
		max_priority = priority[cont_index];

	sort_priority();
}

void UIManager::deactivate_container(std::string name)
{
	int cont_index = -1;
	UIElement* cont = 0;
	for (int i = 0; i < containers.size(); i++)
	{
		if (containers[i]->name == name)
		{
			cont = containers[i];
			cont_index = i;
		}
	}
	if (cont == 0)
		return;

	cont->active = false;
	cont->visible = false;

	if (priority[cont_index] >= max_priority)
		calculate_max_priority();

	sort_priority();
}

UIElement* UIManager::get_element(std::string path)
{
	UIElement* current_element = 0;
	int word_start = 0;
	int word_end = 0;
	std::string word;
	for (int i = 0; i < path.length(); i++)
	{
		if (path[i] == '.')
		{
			word_end = i;
			word = path.substr(word_start, word_end-word_start);
			if (word_start == 0)
				current_element = get_container(word);
			else
				current_element = current_element->get_element(word);
			if (current_element == 0)
				return 0;
			word_start = i + 1;
		}
	}
	// last element
	word = path.substr(word_start, path.length() - word_start);
	if (word_start == 0)
		current_element = get_container(word);
	else
		current_element = current_element->get_element(word);
	return current_element;
}

void UIManager::resolve_event(SDL_Event* ev)
{
	for (int i = 0; i < containers.size(); i++)
		if(priority[i] == max_priority)
			containers[i]->propagate_event(ev);
}

void UIManager::render(SDL_Renderer* renderer)
{
	for (int i = 0; i < containers.size(); i++)
		containers[container_priority[i]]->render(renderer);
}