#include "PulseType.h"

PulseType::PulseType(Event<void>& destroyEvent, Event<void>& startEvent, Event<void>& stopEvent, Event<const uint64_t&>& tickEvent) :
	ControllerType(destroyEvent, startEvent, stopEvent),
	onTickAction(tickEvent.reg(&PulseType::onTick, this))
{
	onTickAction->enable = false;
} //PulseType

PulseType::~PulseType()
{
	onTickAction->unreg();
} //~PulseType

void PulseType::onStart()
{
	onTickAction->enable = true;

	ControllerType::onStart();
} //onStart

void PulseType::onStop()
{
	onTickAction->enable = false;

	ControllerType::onStop();
} //onStop

void PulseType::onTick(const uint64_t& delta)
{
	eventTick.exec(delta);
} //onTick
