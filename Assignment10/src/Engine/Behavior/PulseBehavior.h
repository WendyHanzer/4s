#pragma once

#include "ControlBehavior.h"
#include "PulseBehaviorEvent.h"

template <class T = PulseBehaviorEvent>
class PulseBehavior : public ControlBehavior<T>
{
	protected:
	
		Event<const long double&>::id actionTick;
	
		PulseBehavior(PulseBehaviorEvent& source) :
			ControlBehavior<T>(source),
			actionTick(source.tick.reg(&PulseBehavior::onTick, this))
		{
			actionTick->enable = false;
		} //PulseType
		

		virtual void onTick(const long double& delta)
		{
			this->event.tick.exec(delta);
		} //onTick

		virtual void onStart()
		{
			actionTick->enable = true;

			ControlBehavior<T>::onStart();
		} //onStart
		virtual void onStop()
		{
			actionTick->enable = false;

			ControlBehavior<T>::onStop();
		} //onStop
		
		virtual void link(PulseBehaviorEvent& source)
		{
			actionTick->unreg();
	
			actionTick = source.tick.reg(&PulseBehavior::onTick, this);
	
			ControlBehavior<T>::link(source);
		} //link

	public:
	
		virtual ~PulseBehavior()
		{
			actionTick->unreg();
		} //~PulseBehavior
		
}; //PulseBehavior
