#pragma once

#define GLM_FORCE_RADIANS

#include <string>
#include <btBulletCollisionCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ActorPhysicsComponent.h"
#include "../Doodad/Goal.h"
#include "../../../Utils/PhysicsUtils.h"

class GoalPhysicsComponent : public ActorPhysicsComponent
{
	private:
	
		virtual void onTick(const long double& delta)
		{
			actionTransformationChange->enable = false;
			
			btTransform goalTrans;
			getTransformation(goalTrans);
			
			glm::mat4 goalPosition = convertBulletTransformToGLM(worldTrans);
			glm::mat4 puckPosition = ((Goal&)getController()).getPuck().getTransformation();
			
			//Collision detection here
			//this should be do with collision detection
			if (
				(goalPosition[3][2] - puckPosition[3][2] < 0.1) && (goalPosition[3][2] - puckPosition[3][2] > -0.1) &&
				(puckPosition[3][0] < .5) && (puckPosition[3][0] > -.5) //tinker with this
				)
			{
				/*
				*	stop puck and let goal know that puck was received
				*/
				getBody().setLinearVelocity(0, 0, 0);
				
				((Goal&)getController()).receivePuck();
			} //if
			
			actionTransformationChange->enable = true;
			
			Component::onTick(delta);
		} //onTick
		
	public:
	
		GoalPhysicsComponent(Actor& controller, const std::string name, btCollisionShape* shape = NULL, 
							const btScalar& mass = 0.0f,  const float& restitution = 0.0f, const float& friction = 0.0f,
							const short& group = -1, const short& mask = -1) :
		ActorPhysicsComponent(controller, name, shape, mass, restitution, friction, group, mask)
		{
		} //GoalPhysicsComponent
		
}; //GoalPhysicsComponent
