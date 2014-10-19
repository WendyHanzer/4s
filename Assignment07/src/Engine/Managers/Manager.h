#pragma once

#include "TypeManager.h"
#include "../Engine.h"

class Engine;

class Manager
{
	private:

		Engine& engine;
		TypeManager::Type& type;
		
	protected:
	
		Manager(Engine& engine, const std::type_index& type);

	public:

		virtual ~Manager() { }

		inline TypeManager::Type& getType() { return type; }
		inline Engine& getEngine()			{ return engine; }
		
}; //Manager
