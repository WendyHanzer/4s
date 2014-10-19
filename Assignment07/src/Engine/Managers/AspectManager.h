#pragma once

#include "Manager.h"

class AspectManager : public Manager
{
	private:
	
		TypeManager::Type& aspectType;
		
	protected:

		AspectManager(Engine& engine, const std::type_index& type);
		
	public:
	
		inline TypeManager::Type& getAspectType() { return aspectType; }
		
}; //AspectManager
