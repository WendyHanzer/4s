#pragma once

#include "../../Actor.h"
#include "ActorAIComponent.h"

/*
*	need command types
*		need command manager (id assignment)
*	need command ids
*/

/*
*	manager.setCommand()
*	manager.appendCommand()
*
*	Actor& source
*/

class ActorAIComponent;

class ActorCommand
{
	protected:
	
		Actor& source;
		ActorAIComponent& manager;

	public:
		
		const std::string name;
	
		virtual bool onTick(const long double& delta) { return true; }
	
		ActorCommand(Actor& source, const std::string& name);
		virtual ~ActorCommand() { }
		
}; //ActorCommand
