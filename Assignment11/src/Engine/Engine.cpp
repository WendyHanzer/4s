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
	
	long double convert = 0.000001;
	
	while (active)
	{
		event.tick.exec(convert*clock.restart().asMicroseconds());
	} //while

	running = false;
	event.stop.exec();
} //tick
