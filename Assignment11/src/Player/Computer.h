#pragma once

#include "../Input/InputSystem.h"
#include "../Entity/Actor/Actor.h"
#include "../Entity/Actor/Component/AI/Command/ActorCommandFollow.h"
#include "../Entity/Actor/Component/AI/Command/ActorCommandStop.h"

#include "AI/ComputerAI.h"

class ComputerAI;

class Computer : public Player
{
	private:
	
		ComputerAI* activeAI;
	
		Event<const long double&>::id actionTick;
		
		void onTick(const long double& delta)
		{
			if (ai != activeAI)
			{
				activeAI = ai;
				
				if (activeAI)
				{
					activeAI->init(*this);
				} //if
			} //if
			
			if (activeAI)
			{
				activeAI->onTick(delta, *this);
			} //if
		} //onTick
		
	public:
	
		ComputerAI* ai;
	
		Computer(const std::string& name, Score& score, Paddle& paddle, Goal& goal, Actor& puck, Engine& engine, ComputerAI* ai = NULL) :
		Player(name, score, paddle, goal, puck, engine),
		activeAI(NULL),
		ai(ai)
		{
			actionTick = engine.event.tick.reg(&Computer::onTick, this);
		} //Computer
		
		virtual ~Computer()
		{
			actionTick->unreg();
		} //Computer
}; //Computer
