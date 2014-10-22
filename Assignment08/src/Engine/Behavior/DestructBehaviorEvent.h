#pragma once

#include "BehaviorEvent.h"

struct DestructBehaviorEvent : protected BehaviorEvent
{
	Event<void> destroy;
	
	DestructBehaviorEvent() : BehaviorEvent() { destroy.reverse(); }
}; //DestructBehaviorEvent
