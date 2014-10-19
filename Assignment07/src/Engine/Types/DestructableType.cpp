#include "DestructableType.h"

DestructableType::DestructableType(Event<void>& destroyEvent) :
	onDestroyAction(destroyEvent.reg(&DestructableType::onDestroy, this))
{
	eventDestroy.reverse();
} //Manager

DestructableType::~DestructableType()
{
	onDestroyAction->unreg();

	eventDestroy.exec();
} //~Manager

void DestructableType::onDestroy()
{
	delete this;
} //onDestroy
