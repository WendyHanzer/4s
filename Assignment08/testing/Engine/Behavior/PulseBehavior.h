#pragma once

#include "ControlBehavior.h"
#include "PulseBehaviorEvent.h"

class PulseBehavior : public ControlBehavior
{
	private:

		Event<const uint64_t&>::id actionTick;

	protected:
	
		PulseBehavior(PulseBehaviorEvent& source);

		virtual void onTick(const uint64_t&);

		virtual void onStart();
		virtual void onStop();

	public:

		virtual ~PulseBehavior();
		
		PulseBehaviorEvent event;
}; //PulseBehavior
