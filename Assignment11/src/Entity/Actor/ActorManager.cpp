#include "ActorManager.h"

ActorManager::ActorManager(Engine& engine) : EntityManager(engine.event)
{
} //ActorManager

ActorManager::~ActorManager()
{
	destroy();
} //~ActorManager
