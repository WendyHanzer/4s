#pragma once

#include "ComponentContext.h"
#include "Component.h"

class Component;

class ComponentManager
{
	friend class Component;

	private:

		Component** components;

	public:

		ComponentManager(ComponentContext& context) : components(new Component*[(const unsigned int&)context]), context(context)
		{
		} //ComponentManager
		
		~ComponentManager() { delete[] components; }
		
		ComponentContext& context;
		
		inline bool hasComponent(const std::type_index& type)
		{
			return components[(const unsigned int&)context[type]] != NULL;
		} //hasComponent

		inline Component& operator[](const std::type_index& type)
		{
			return *components[(const unsigned int&)context[type]];
		} //[]
		
		inline Component& operator[](const unsigned int& type)
		{
			return *components[type];
		} //[]
}; //ComponentManager
