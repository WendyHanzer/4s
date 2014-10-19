#include "AspectSource.h"

AspectSource::AspectSource(AspectSourceManager& manager) :
	ControllerType(dynamic_cast<ControllerType&>(manager).eventDestroy, dynamic_cast<ControllerType&>(manager).eventStart, dynamic_cast<ControllerType&>(manager).eventStop),
	aspectTypes(manager.aspectTypes)
{
	aspects = new Aspect*[(const unsigned int&)aspectTypes + 1];
	
	for (unsigned int i = 0; i <= (const unsigned int&)aspectTypes; ++i)
	{
		aspects[i] = NULL;
	} //for
} //AspectSource

AspectSource::AspectSource(ControllerType& controller, AspectSourceManager& manager) :
	ControllerType(controller.eventDestroy, controller.eventStart, controller.eventStop),
	aspectTypes(manager.aspectTypes)
{
	aspects = new Aspect*[(const unsigned int&)aspectTypes];
	
	for (unsigned int i = 0; i <= (const unsigned int&)aspectTypes; ++i)
	{
		aspects[i] = NULL;
	} //for
} //AspectSource

AspectSource::~AspectSource()
{
	delete[] aspects;
} //~AspectSource
