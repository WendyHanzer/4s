#include "ActiveType.h"

ActiveType::ActiveType(Event<void>& startEvent, Event<void>& stopEvent) :
	onStartAction(startEvent.reg(&ActiveType::start, this)),
	onStopAction(stopEvent.reg(&ActiveType::stop, this)),
	active(false)
{
} //ActiveManager

ActiveType::~ActiveType()
{
	onStartAction->unreg();
	onStopAction->unreg();
} //~ActiveManager

void ActiveType::start()
{
	if (!active)
	{
		onStart();
	} //if
} //start

void ActiveType::stop()
{
	if (active)
	{
		onStop();
	} //if
} //stop

void ActiveType::onStart()
{
	active = true;
	eventStart.exec();
} //onStart

void ActiveType::onStop()
{
	active = false;
	eventStop.exec();
} //onStop
