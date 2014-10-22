#include "SceneManager.h"

SceneManager::SceneManager(Engine& engine) :
	AspectSourceManager(engine, typeid(SceneManager)),
	ControllerType(engine.eventDestroy, engine.eventStart, engine.eventStop)
{
} //SceneManager
