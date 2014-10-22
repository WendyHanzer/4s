#pragma once

#include <stdint.h>

#include "ControlBehaviorEvent.h"

struct PulseBehaviorEvent : public ControlBehaviorEvent
{
	Event<const uint64_t&> tick;
	
	PulseBehaviorEvent() : ControlBehaviorEvent() { }
}; //PulseBehaviorEvent
