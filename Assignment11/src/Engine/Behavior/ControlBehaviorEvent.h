#pragma once

#include "DestructBehaviorEvent.h"

struct ControlBehaviorEvent : public DestructBehaviorEvent
{
	Event<void> start;
	Event<void> stop;
	
	ControlBehaviorEvent() : DestructBehaviorEvent()
	{
		stop.reverse();
	} //ControlBehaviorEvent
}; //ControlBehaviorEvent
