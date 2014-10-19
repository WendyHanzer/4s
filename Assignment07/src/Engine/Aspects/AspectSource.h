#pragma once

#include "Aspect.h"
#include "../Types/ControllerType.h"
#include "../Managers/AspectSourceManager.h"

class Aspect;

class AspectSource : public ControllerType
{	
	protected:
		
		AspectSource(AspectSourceManager& manager);
		AspectSource(ControllerType& controller, AspectSourceManager& manager);
		
	public:
	
		virtual ~AspectSource();
	
		Aspect** aspects;
		TypeManager& aspectTypes;
		
}; //AspectSource
