#include "DestructBehavior.h"

DestructBehavior::DestructBehavior(DestructBehaviorEvent& source) :
	Behavior(),
	actionDestroy(source.destroy.reg(&DestructBehavior::onDestroy, this))
{
} //DestructBehavior

DestructBehavior::~DestructBehavior()
{
	actionDestroy->unreg();

	event.destroy.exec();
} //~DestructBehavior

void DestructBehavior::onDestroy()
{
	delete this;
} //onDestroy
