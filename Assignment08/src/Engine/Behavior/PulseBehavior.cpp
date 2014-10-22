#include "PulseBehavior.h"

PulseBehavior::PulseBehavior(PulseBehaviorEvent& source) :
	ControlBehavior(source),
	actionTick(source.tick.reg(&PulseBehavior::onTick, this))
{
	actionTick->enable = false;
} //PulseType

PulseBehavior::destroy()
{
	actionTick->unreg();
	ControlBehavior::destroy();
} //destroy

void PulseBehavior::onStart()
{
	actionTick->enable = true;

	ControlBehavior::onStart();
} //onStart

void PulseBehavior::onStop()
{
	actionTick->enable = false;

	ControlBehavior::onStop();
} //onStop

void PulseBehavior::onTick(const uint64_t& delta)
{
	event.tick.exec(delta);
} //onTick
