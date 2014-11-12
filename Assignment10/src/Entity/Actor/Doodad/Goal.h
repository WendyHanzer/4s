#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Actor.h"

class Goal : public Actor
{
	private:
	
		Actor* puck;
		
	public:
	
		Event<void> receive;
	
		Goal(ActorManager& actorManager, const std::string name, Actor& puck) :
		Actor(actorManager, name),
		puck(&puck)
		{
		} //Goal
		
		void receivePuck()
		{
			/*
			*	let rest of game know that a puck has been received from this goal
			*/
			receive.exec();
		} //receivePuck
		
		inline Actor& getPuck()
		{
			return *puck;
		} //getPuck
};
