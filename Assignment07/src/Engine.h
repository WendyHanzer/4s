#pragma once

#include <vector>
#include <chrono>
#include <typeinfo>

#include "Utils/Event.h"
#include "Managers/TypeManager.h"
#include "Managers/Manager.h"

class TypeManager;
class Manager;

/*
*	Get rid of static initialization
*
*	Extend Engine for specific type of Engine and do
*	engine specific initialization in the virtual init method.
*
*	Aspect types still run through static initialization.
*
*	Create all desired managers (no need for init on a manager now)
*
*	Create all scenes and bind them to SceneManager, setting first scene
*
*	There are two ways to manage scene loading/initialization
*
*		*SERVER SIDE*
*		On initialization, create the entire scene state and save it
*		On load, enable the scene for rendering
*		On unload, disable the scene for rendering
*
*		*CLIENT SIDE*
*		On initialization, do nothing
*		On load, create the entire scene state and enable for rendering
*		On unload, destroy the entire scene state and disable for rendering
*
*	KEY: No OnInit for Managers! It's just the constructor!
*		 No more engine eventInit either!
*/

class Engine { /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
			std::vector<Manager*> managers;
			TypeManager managerTypes;

			std::chrono::time_point<std::chrono::high_resolution_clock> previousTime;
			std::chrono::time_point<std::chrono::high_resolution_clock> currentTime;\

			bool running;
/*
*		Methods
*		----------------------------------
*/
			void tick();

/*
*		Types
*		----------------------------------
*/
			class ManagerManager : public TypeManager
			{
				public:

					ManagerManager() : TypeManager(NULL, typeid(ManagerManager)) { }
			}
/*
*****************************************************************
*
*/	public: /*
*
*		Constructors
*		----------------------------
*/
			Engine();
			~Engine();
/*
*		Fields
*		----------------------------------
*/
			Event<void> eventDestroy;
			Event<void> eventStart;
			Event<void> eventStop;
			Event<double> eventTick;

			ManagerManager aspectTypes;
/*
*		Methods
*		----------------------------------
*/
			virtual void init() = 0;

			void start();
			inline void stop() { running = false; }

			TypeManager::Type* addManager(std::type_info managerType, Manager* manager);
			inline Manager* getManager(const TypeManager::Type* type) { return managers[type->id]; }

			inline TypeManager::Type* getManagerType(std::type_info managerType) { return managerTypes.get(managerType); }
/*
*****************************************************************/
};