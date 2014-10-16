#include "Engine.h"

/*
*	Constructors
*/
Engine::Engine()
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
void Engine::start()
{
	running = true;

	tick();
} //start

Engine::tick()
{
	double delta;

	eventStart.exec();

	previousTime = std::chrono::high_resolution_clock::now();
	while (running)
	{
		currentTime = std::chrono::high_resolution_clock::now();
		delta = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - previousTime).count();
		eventTick.exec(delta);
		previousTime = currentTime;
	} //while

	eventStop.exec();
} //tick

TypeManager::Type* Engine::addManager(std::type_info managerType, Manager* manager)
{
	TypeManager::Type* type = managerTypes.create(managerType);

	if (type.id >= managers.capacity())
	{
		managers.reserve(managers.capacity() << 1);
	} //if

	managers[type->id] = manager;

	return type;
} //addManager