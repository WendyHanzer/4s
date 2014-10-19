#include "Manager.h"

Manager::Manager(Engine& engine, const std::type_index& type) :
	engine(engine),
	type(engine.managers.add(*this, type))
{
} //Manager
