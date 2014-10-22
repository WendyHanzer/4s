#pragma once

#include "DestructBehavior.h"
#include "ControlBehaviorEvent.h"

class ControlBehavior : public DestructBehavior
{
	private:

		Event<void>::id actionStart;
		Event<void>::id actionStop;

		bool running;
		
	protected:
	
		ControlBehavior(ControlBehaviorEvent& source);
	
		virtual void destroy();
		virtual void onStart();
		virtual void onStop();

	public:

		ControlBehaviorEvent event;

		void start();
		void stop();

		inline const bool& isRunning() { return running; }
}; //ControlBehavior
