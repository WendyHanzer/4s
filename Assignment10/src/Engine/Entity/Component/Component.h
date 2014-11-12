#pragma once

#include "../Entity.h"
#include "../../Behavior/PulseBehavior.h"

class Entity;
class ComponentManager;

class Component : public PulseBehavior<>
{
	private:
	
		Entity& controller;						//the entity
												//may be Entity Component, System, or Engine

		PulseBehavior<>& componentController;	//the space where the entity exists
		
		Event<void>::id actionChangeController;
		Event<void>::id actionStart;
		Event<void>::id actionStop;
		Event<void>::id actionDestroy;

	protected:
	
		void setController(PulseBehavior<>& componentController);
		
		virtual void onChangeController() = 0;
		Component(Entity& controller, PulseBehavior<>& componentController, const std::type_index& type, const std::string& name);

	public:

		virtual ~Component();
		
		const std::string name;
		
		Event<void> eventChangeController;

		ComponentContext::Mapping& mapping;		//this component's mapping to system
		
		inline Entity& getController() { return controller; }
		inline PulseBehavior<>& getComponentController() { return componentController; }
		
}; //Component
