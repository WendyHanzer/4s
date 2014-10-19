#pragma once

#include "Manager.h"

class AspectSourceManager : public Manager
{
	protected:
	
		AspectSourceManager(Engine& engine, const std::type_index& type);

	public:
	
		TypeManager aspectTypes;
		
}; //AspectSourceManager
