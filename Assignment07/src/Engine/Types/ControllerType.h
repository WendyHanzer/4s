#pragma once

#include "DestructableType.h"
#include "ActiveType.h"

class ControllerType : public DestructableType, public ActiveType
{
	protected:

		ControllerType(Event<void>& destroyEvent, Event<void>& startEvent, Event<void>& stopEvent);

}; //ControllerType

