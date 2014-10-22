#pragma once

#include <SFML/System/Clock.hpp>

#include "Behavior/PulseBehaviorEvent.h"

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
	
			const Engine& operator=(const Engine& engine) { return *this; }
/*
*****************************************************************
*
*/	protected: /*
*
*		Constructors
*		----------------------------
*/
			Engine();
/*
*****************************************************************
*
*/	public: /*
*
*		Constructors
*		----------------------------
*/
			virtual ~Engine();
/*
*		Fields
*		----------------------------------
*/
			PulseBehaviorEvent event;
/*
*		Methods
*		----------------------------------
*/
			inline void start() { tick(); }
			inline void stop() { active = false; }

			inline const bool& isRunning() { return running; }
/*
*****************************************************************/
};
