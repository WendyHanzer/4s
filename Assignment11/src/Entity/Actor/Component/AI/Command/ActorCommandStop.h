#pragma once

#include "../ActorCommand.h"
#include "../../ActorGraphicsComponent.h"

class ActorCommandStop : public ActorCommand
{
	public:
	
		virtual bool onTick(const long double& delta)
		{
			return true;
		} //onTick
	
		ActorCommandStop(Actor& source, const std::string& name = "stop") : ActorCommand(source, name)
		{
			manager.setCommand(*this);
		} //ActorCommandStop
		
}; //ActorCommandStop
