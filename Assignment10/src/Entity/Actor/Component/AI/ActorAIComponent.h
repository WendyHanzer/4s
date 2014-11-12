#pragma once

#include <list>

#include "../../../../Engine/Entity/Component/Component.h"
#include "../../../../AI/AISystem.h"
#include "../../Actor.h"
#include "ActorCommand.h"

class ActorCommand;

class ActorAIComponent : public Component
{
	protected:
	
		std::list<ActorCommand*> commands;

		virtual void onChangeController()
		{
		} //onChangeController

		virtual void onTick(const long double& delta);

	public:

		ActorAIComponent(Actor& controller, const std::string& name);
		virtual ~ActorAIComponent();
		
		void clearCommands();
		void setCommand(ActorCommand& command);
		
		inline void appendCommand(ActorCommand& command)
		{
			commands.push_back(&command);
		} //appendCommand
		
}; //ActorAIComponent
