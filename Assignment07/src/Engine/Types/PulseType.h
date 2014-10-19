#pragma once

#include "ControllerType.h"

class PulseType : public ControllerType
{
	private:

		Event<const uint64_t&>::id onTickAction;

	protected:
	
		PulseType(Event<void>& destroyEvent, Event<void>& startEvent, Event<void>& stopEvent, Event<const uint64_t&>& tickEvent);

		virtual void onTick(const uint64_t& delta);

		virtual void onStart();
		virtual void onStop();

	public:

		virtual ~PulseType();

		Event<const uint64_t&> eventTick;

}; //PulseType
