#pragma once

#include "Behavior.h"
#include "DestructBehaviorEvent.h"

class DestructBehavior : public Behavior
{
	private:

		Event<void>::id actionDestroy;
		void onDestroy();
		
	protected:
	
		DestructBehavior(DestructBehaviorEvent& source);

	public:
	
		DestructBehaviorEvent event;

		virtual ~DestructBehavior();

}; //DestructBehavior
