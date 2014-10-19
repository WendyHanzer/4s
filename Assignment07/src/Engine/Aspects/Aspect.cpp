#include "Aspect.h"

Aspect::Aspect(AspectSource& source, TypeManager::Type& type, PulseType& controller) :
	onDestroyActionSource(source.eventDestroy.reg(&Aspect::onDestroy, this)),
	onDestroyActionController(controller.eventDestroy.reg(&Aspect::onDestroy, this)),

	onStartActionSource(source.eventStart.reg(&Aspect::start, this)),
	onStartActionController(controller.eventStart.reg(&Aspect::start, this)),
	
	onStopActionSource(source.eventStop.reg(&Aspect::stop, this)),
	onStopActionController(controller.eventStop.reg(&Aspect::stop, this)),

	onTickAction(controller.eventTick.reg(&Aspect::onTick, this)),
	
	type(type),
	source(source),
	controller(controller),
	
	active(false)
{
	source.aspects[(unsigned const int&)type] = this;
} //Aspect

Aspect::~Aspect()
{
	onDestroyActionSource->unreg();
	onDestroyActionController->unreg();
	onStartActionSource->unreg();
	onStartActionController->unreg();
	onStopActionSource->unreg();
	onStopActionController->unreg();
	onTickAction->unreg();
	
	source.aspects[(unsigned const int&)type] = NULL;
} //~Aspect

void Aspect::start()
{
	if (!active && source.isActive() && controller.isActive())
	{
		onStart();
	} //if
} //start

void Aspect::stop()
{
	if (active && (!source.isActive() || !controller.isActive()))
	{
		onStop();
	} //if
} //stop

void Aspect::onStart()
{
	active = true;
} //onStart

void Aspect::onStop()
{
	active = false;
} //onStop

void Aspect::onDestroy()
{
	delete this;
} //onDestroy
