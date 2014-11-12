#pragma once

#include "../Actor.h"

#include "../Component/ActorPhysicsComponent.h"

class Plane : public Actor
{
	private:
		
		
	public:
		Plane(ActorManager& actorManager, const std::string name, const btVector3& normal, const short& group = -1, const short& mask = -1) :
		
		Actor(actorManager, name)
		{
			new ActorPhysicsComponent(*this, "Physics Component", new btStaticPlaneShape(normal, 1), 0, .05f, .15f, group, mask);
		} //Plane
		
		virtual ~Plane()
		{
			destroy();
		} //~Plane
};
