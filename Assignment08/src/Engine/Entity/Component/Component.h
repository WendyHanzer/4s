#pragma once

#include "../Entity.h"
#include "../../Types/Behavior/PulseBehavior.h"

class ComponentManager;

class Component : public PulseBehavior
{
	private:
	
		Entity& controller;						//the entity
		PulseBehavior& componentController;		//the space where the entity exists
												//may be Entity Component, System, or Engine
		
		Event<void>::id actionChangeController;
		Event<void>::id actionStart;
		Event<void>::id actionStop;
		Event<void>::id actionDestroy;

	protected:

		virtual void onChangeController() = 0;
		
		Component(Entity& controller, PulseBehavior& componentController, const std::type_index& type);

	public:

		virtual ~Component();
		
		Event<void> eventChangeController;

		ComponentContext::Mapping& mapping;		//this component's mapping to system
		
		Entity& getController() { return controller; }
		Entity& getComponentController() { return componentController; }
}; //Component
