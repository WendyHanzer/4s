#include "Engine.h"

/*
*	Constructors
*/
Engine::Engine() : running(false), active(false)
{
} // Engine

Engine::~Engine()
{
	event.destroy.exec();
} // ~Engine

/*
*	Methods
*/
void Engine::tick()
{
	active = true;
	running = true;

	event.start.exec();

	clock.restart();
	while (active)
	{
		event.tick.exec(clock.restart().asMicroseconds());
	} //while

	running = false;
	event.stop.exec();
} //tick
