#include "ActorCommand.h"

ActorCommand::ActorCommand(Actor& source, const std::string& name) :
	source(source),
	manager(((ActorAIComponent&)source.components[typeid(AISystem)])),
	name(name)
{
} //ActorCommand
