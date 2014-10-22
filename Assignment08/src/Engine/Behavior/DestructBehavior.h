#pragma once

#include "Behavior.h"
#include "DestructBehaviorEvent.h"

class DestructBehavior : public Behavior
{
	private:

		Event<void>::id actionDestroy;
		
	protected:
		
		void onDestroy();
		DestructBehavior(DestructBehaviorEvent& source);

	public:
	
		DestructBehaviorEvent event;

		virtual ~DestructBehavior();

}; //DestructBehavior
