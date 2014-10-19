#pragma once

#include "../../Utils/Event.h"

class ActiveType
{
	private:

		Event<void>::id onStartAction;
		Event<void>::id onStopAction;

		bool active;
		
	protected:
	
		ActiveType(Event<void>& startEvent, Event<void>& stopEvent);
	
		virtual void onStart();
		virtual void onStop();

	public:
	
		virtual ~ActiveType();

		Event<void> eventStart;
		Event<void> eventStop;

		void start();
		void stop();

		inline const bool& isActive() { return active; }
}; //ActiveType
