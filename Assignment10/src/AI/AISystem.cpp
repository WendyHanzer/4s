#include "AISystem.h"

AISystem::AISystem(Engine& engine) :
	System(engine)
{
} //AISystem

AISystem::~AISystem()
{
	destroy();
} //~AISystem
