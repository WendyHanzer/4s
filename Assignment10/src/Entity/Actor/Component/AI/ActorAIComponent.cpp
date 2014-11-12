#include "ActorAIComponent.h"

void ActorAIComponent::onTick(const long double& delta)
{
	if (!commands.empty() && commands.front()->onTick(delta))
	{
		commands.pop_front();
	} //if

	Component::onTick(delta);
} //onTick

ActorAIComponent::ActorAIComponent(Actor& controller, const std::string& name) :
	Component(controller, (System&)controller.getManager().context[typeid(AISystem)], typeid(AISystem), name)
{
} //ActorAIComponent

ActorAIComponent::~ActorAIComponent()
{
	destroy();
	
	clearCommands();
} //~ActorAIComponent

void ActorAIComponent::clearCommands()
{
	for (auto c : commands)
	{
		delete c;
	} //for
	
	commands.clear();
} //clearCommands

void ActorAIComponent::setCommand(ActorCommand& command)
{
	clearCommands();
	
	appendCommand(command);
} //setCommand
