#pragma once

#include "../ActorCommand.h"
#include "../../ActorPhysicsComponent.h"
#include "../../../../../Utils/PositionUtils.h"

class ActorCommandFollow : public ActorCommand
{
	public:
	
		Actor& target;
		btRigidBody& body;
		float maxSpeed;
		float leash;
		
		btVector3 currentSpeed;
	
		virtual bool onTick(const long double& delta)
		{
			const glm::mat4& sourceTransformation = source.getTransformation();
			const glm::mat4& targetTransformation = target.getTransformation();
			
			glm::vec3 sourcePosition(sourceTransformation[3][0], 0, sourceTransformation[3][2]);
			glm::vec3 targetPosition(targetTransformation[3][0], 0, targetTransformation[3][2]);
			glm::vec3 direction = (targetPosition - sourcePosition);
			
			//glm::quat orientation(quatBetweenVectors(sourcePosition, targetPosition));
			float distance(glm::length(direction));
			
			/*
			*	this is used to artificially increase speed
			*/
			if (leash < 0)
			{
				distance -= leash;
			} //if
			
			if (distance != 0)
			{
				direction = glm::normalize(direction);
			} //if
			
			if (distance <= leash)
			{
				currentSpeed.setX(0);
				currentSpeed.setY(body.getLinearVelocity().getY());
				currentSpeed.setZ(0);
				
				body.setLinearVelocity(currentSpeed);
				
				return false;
			} //if
			
			float speed = glm::length(distance)/delta/10;
			float theta = atan2(direction.z, direction.x);
			
			
			if (maxSpeed > -1)
			{
				speed = maxSpeed;
			} //if
			
			//was doing weird circling with quaternions
			//newSpeed = glm::normalize(newSpeed);
			//newSpeed = orientation*newSpeed;
			
			//body.setLinearVelocity(body.getLinearVelocity() - currentSpeed);
			
			currentSpeed.setX(speed*cos(theta));
			currentSpeed.setY(body.getLinearVelocity().getY());
			currentSpeed.setZ(speed*sin(theta));
			
			body.setLinearVelocity(currentSpeed);
		
			return false;
		} //onTick
	
		ActorCommandFollow(Actor& source, const std::string& name, Actor& target, const float& maxSpeed = -1, const float& leash = 0) :
			ActorCommand(source, name),
			target(target),
			body(((ActorPhysicsComponent&)source.components[typeid(PhysicsSystem)]).getBody()),
			maxSpeed(maxSpeed),
			leash(leash),
			currentSpeed(0, 0, 0)
		{
			manager.setCommand(*this);
		} //ActorCommandFollow
	
}; //ActorCommandFollow
