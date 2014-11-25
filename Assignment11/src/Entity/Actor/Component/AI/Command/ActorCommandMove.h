#pragma once

#include "../ActorCommand.h"
#include "../../ActorPhysicsComponent.h"
#include "../../../../../Utils/PositionUtils.h"

class ActorCommandMove : public ActorCommand
{
	public:
	
		const glm::vec3 targetPosition;
		btRigidBody& body;
		float maxSpeed;
		float leash;
		
		btVector3 currentSpeed;
	
		virtual bool onTick(const long double& delta)
		{
			const glm::mat4& sourceTransformation = source.getTransformation();
			
			glm::vec3 sourcePosition(sourceTransformation[3][0], 0, sourceTransformation[3][2]);
			glm::vec3 direction = (targetPosition - sourcePosition);
			float distance(glm::length(direction));
			
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
				
				return true;
			} //if
			
			float speed = glm::length(distance)/delta/10;
			float theta = atan2(direction.z, direction.x);
			
			if (maxSpeed > -1)
			{
				speed = maxSpeed;
			} //if
			
			currentSpeed.setX(speed*cos(theta));
			currentSpeed.setY(body.getLinearVelocity().getY());
			currentSpeed.setZ(speed*sin(theta));
			
			body.setLinearVelocity(currentSpeed);
		
			return false;
		} //onTick
	
		ActorCommandMove(Actor& source, const std::string& name, const glm::vec3& targetPosition, const float& maxSpeed = -1, const float& leash = .05) :
			ActorCommand(source, name),
			targetPosition(targetPosition.x, 0, targetPosition.z),
			body(((ActorPhysicsComponent&)source.components[typeid(PhysicsSystem)]).getBody()),
			maxSpeed(maxSpeed),
			leash(leash),
			currentSpeed(0, 0, 0)
		{
			manager.setCommand(*this);
		} //ActorCommandMove
	
}; //ActorCommandMove
