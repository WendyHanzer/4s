#pragma once

#include "../Behavior/ControlBehavior.h"
#include "../System/IdManager.h"
#include "Component/ComponentContext.h"

class EntityManager : public ControlBehavior
{	
	protected:
		
		//controller is where the entity exists
		//context is what managers the entity, gives it ids
		EntityManager(ControlBehaviorEvent& controller) : ControlBehavior(controller) { }
		
	public:
	
		virtual ~EntityManager() { }
	
		ComponentContext context;
		IdManager idManager;
		
}; //EntityManager
