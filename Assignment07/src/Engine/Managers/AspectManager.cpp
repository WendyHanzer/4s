#include "AspectManager.h"

AspectManager::AspectManager(Engine& engine, const std::type_index& type) :
	Manager(engine, type),
	aspectType(engine.aspectTypes.create(type))
{
} //AspectManager
