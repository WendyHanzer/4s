#pragma once

#include <string>

#include "Component/ComponentManager.h"
#include "EntityManager.h"

class Component;

class Entity : public ControlBehavior
{
	private:
	
		ControlBehavior* controller;
		EntityManager* manager;
		
		Event<void>::id actionStart;
		Event<void>::id actionStop;
		Event<void>::id actionDestroy;

	protected:
		
		//controller is where the entity exists
		Entity(ControlBehavior& controller, EntityManager& manager, const std::string name);
		
	public:
	
		virtual ~Entity();
		
		Event<void> eventChangeController;
	
		const std::string name;
		const unsigned int id;
	
		ComponentManager components;
		
		inline ControlBehavior& getController()
		{
			return *controller;
		} //getController
		
		inline void setController(ControlBehavior& controller)
		{
			this->controller = &controller;
			
			eventChangeController.exec();
		} //setController
		
		inline EntityManager& getManager()
		{
			return *manager;
		} //getManager()
		
}; //Entity
