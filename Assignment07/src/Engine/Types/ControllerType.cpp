#include "ControllerType.h"

ControllerType::ControllerType(Event<void>& destroyEvent, Event<void>& startEvent, Event<void>& stopEvent) :
	DestructableType(destroyEvent),
	ActiveType(startEvent, stopEvent)
{
} //ControllerType
