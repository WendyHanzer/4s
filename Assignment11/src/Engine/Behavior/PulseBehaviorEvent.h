#pragma once

#include <stdint.h>

#include "ControlBehaviorEvent.h"

struct PulseBehaviorEvent : public ControlBehaviorEvent
{
	Event<const long double&> tick;
	
	PulseBehaviorEvent() : ControlBehaviorEvent() { }
}; //PulseBehaviorEvent
