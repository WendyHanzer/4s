#include "ControlBehavior.h"

ControlBehavior::ControlBehavior(ControlBehaviorEvent& source) :
	DestructBehavior(source),
	actionStart(source.start.reg(&ControlBehavior::start, this)),
	actionStop(source.stop.reg(&ControlBehavior::stop, this)),
	running(false)
{
} //ControlBehavior

ControlBehavior::~ControlBehavior()
{
	actionStart->unreg();
	actionStop->unreg();
} //~ControlBehavior

void ControlBehavior::start()
{
	if (!running)
	{
		onStart();
	} //if
} //start

void ControlBehavior::stop()
{
	if (running)
	{
		onStop();
	} //if
} //stop

void ControlBehavior::onStart()
{
	running = true;
	event.start.exec();
} //onStart

void ControlBehavior::onStop()
{
	running = false;
	event.stop.exec();
} //onStop
