#pragma once

#include "DestructBehavior.h"
#include "ControlBehaviorEvent.h"

template <class T = ControlBehaviorEvent>
class ControlBehavior : public DestructBehavior<T>
{
	protected:

		Event<void>::id actionStart;
		Event<void>::id actionStop;
		
	private:

		bool running;
		
	protected:
	
		ControlBehavior(ControlBehaviorEvent& source) :
			DestructBehavior<T>(source),
			actionStart(source.start.reg(&ControlBehavior::start, this)),
			actionStop(source.stop.reg(&ControlBehavior::stop, this)),
			running(false)
		{
		} //ControlBehavior
	
		virtual void onStart()
		{
			running = true;
			this->event.start.exec();
		} //onStart
		
		virtual void onStop()
		{
			running = false;
			this->event.stop.exec();
		} //onStop
	
		virtual void link(ControlBehaviorEvent& source)
		{
			actionStart->unreg();
			actionStop->unreg();
	
			actionStart = source.start.reg(&ControlBehavior::start, this);
			actionStop = source.stop.reg(&ControlBehavior::stop, this);
	
			DestructBehavior<T>::link(source);
		} //link

	public:
	
		virtual ~ControlBehavior()
		{
			actionStart->unreg();
			actionStop->unreg();
		} //~ControlBehavior

		void start()
		{
			if (!running)
			{
				onStart();
			} //if
		} //start
		
		void stop()
		{
			if (running)
			{
				onStop();
			} //if
		} //stop

		inline const bool& isRunning() { return running; }
		
}; //ControlBehavior
