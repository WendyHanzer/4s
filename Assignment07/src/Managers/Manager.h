#pragma once

#include <typeinfo>

#include "TypeManager.h"
#include "../Engine.h"
#include "../Utils/Event.h"

class TypeManager;
class Engine;

class Manager
{
	private:

		TypeManager::Type* type;
		Event::id onDestroyAction;

	public:

		Manager(Engine* engine, std::type_info type);
		virtual ~Manager();

		Event<void> eventDestroy;

		inline TypeManager::Type* getType() { return type; }
}; //Manager