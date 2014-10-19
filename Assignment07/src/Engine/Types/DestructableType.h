#pragma once

#include "../../Utils/Event.h"

class DestructableType
{
	private:

		Event<void>::id onDestroyAction;
		void onDestroy();
		
	protected:
	
		DestructableType(Event<void>& destroyEvent);

	public:

		virtual ~DestructableType();

		Event<void> eventDestroy;

}; //DestructableType
