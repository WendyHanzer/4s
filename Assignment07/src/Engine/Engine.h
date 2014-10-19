#pragma once

#include <stdint.h>
#include <SFML/System/Clock.hpp>

#include "../Utils/Event.h"
#include "Managers/Manager.h"
#include "Managers/TypeManager.h"
#include "TypeVector.h"

class Manager;

class Engine { /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
			sf::Clock clock;

			bool running;
			bool active;
/*
*		Methods
*		----------------------------------
*/
			void tick();
/*
*****************************************************************
*
*/	public: /*
*
*		Constructors
*		----------------------------
*/
			Engine();
			virtual ~Engine();
/*
*		Fields
*		----------------------------------
*/
			TypeVector<Manager> managers;
			TypeManager aspectTypes;

			Event<void> eventDestroy;
			Event<void> eventStart;
			Event<void> eventStop;
			Event<const uint64_t&> eventTick;
/*
*		Methods
*		----------------------------------
*/
			virtual void init() = 0;

			inline void start() { tick(); }
			inline void stop() { running = false; }

			inline const bool& isActive() { return active; }
/*
*****************************************************************/
};
