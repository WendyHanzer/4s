#pragma once

#include "../Behavior/PulseBehavior.h"
#include "../Engine.h"

class System : public PulseBehavior
{
	private:
	
		const System& operator=(const System& system) { return *this; }
		
	protected:

		System(Engine& engine) : PulseBehavior(engine.event), engine(engine) { }
		
	public:
	
		Engine& engine;
		
}; //System
