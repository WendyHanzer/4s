#pragma once

#include "Behavior.h"
#include "DestructBehaviorEvent.h"

template <class T = DestructBehaviorEvent>
class DestructBehavior : public Behavior<T>
{
	protected:

		Event<void>::id actionDestroy;
		
	private:
		
		bool alive;
		
	protected:
	
		DestructBehavior(DestructBehaviorEvent& source) :
			Behavior<T>(),
			actionDestroy(source.destroy.reg(&DestructBehavior::onDestroy, this)),
			alive(true)
		{
		} //DestructBehavior
		
		void onDestroy()
		{
			delete this;
		} //onDestroy
		
		void destroy()
		{
			if (alive)
			{
				alive = false;
		
				this->event.destroy.exec();
			} //if
		} //destroy
	
		virtual void link(DestructBehaviorEvent& source)
		{
			actionDestroy->unreg();
	
			actionDestroy = source.destroy.reg(&DestructBehavior::onDestroy, this);
		} //link

	public:

		virtual ~DestructBehavior()
		{
			actionDestroy->unreg();
		} //~DestructBehavior

}; //DestructBehavior
