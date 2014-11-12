#pragma once

#include "BehaviorEvent.h"

template <class T = void*>
class Behavior
{
	protected:

		Behavior() { }

	public:

		virtual ~Behavior() { }
		
		T event;

}; //Behavior
