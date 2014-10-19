#include "Engine.h"

/*
*	Constructors
*/
Engine::Engine() : running(false), active(false)
{
	eventDestroy.reverse();
} // Engine

Engine::~Engine()
{
	eventDestroy.exec();
} // ~Engine

/*
*	Methods
*/
void Engine::tick()
{
	active = true;
	running = true;

	eventStart.exec();

	clock.restart();
	while (running)
	{
		eventTick.exec(clock.restart().asMicroseconds());
	} //while

	active = false;
	eventStop.exec();
} //tick
