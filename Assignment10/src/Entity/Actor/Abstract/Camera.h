#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>

#include "../../../Utils/PositionUtils.h"

#include "../Actor.h"

class Camera : public Actor
{
	private:
	
		glm::vec3 up;
		glm::vec3 position;
		glm::quat orientationX;
		glm::quat orientationY;
		glm::quat orientation;
		
	public:
	
		Actor* target;
	
		Camera(ActorManager& actorManager, const std::string name = "Camera", Actor* target = NULL) :
		Actor(actorManager, name),
		up(glm::vec3(0, 1, 0)),
		target(target)
		{
		} //Camera
		
		const glm::mat4 getView()
		{
			if (target)
			{
				const glm::mat4 targetTransformation = target->getTransformation();
				
				glm::vec4 worldPosition = targetTransformation*glm::vec4(position, 1);
			
				return glm::lookAt
				(
					glm::vec3(worldPosition.x, worldPosition.y, worldPosition.z),
					glm::vec3(targetTransformation[3][0], targetTransformation[3][1], targetTransformation[3][2]),
					up
				);
			} //if

			return glm::toMat4(orientation)*glm::translate(glm::mat4(1.0f), position);
		} //getView
		
		//inline glm::quat getRotation() { return orientation; }
		
		inline const glm::vec3& getPosition()
		{
			return position;
		} //getPosition
		
		inline const float getZoom()
		{
			if (target || true)
			{
				return glm::length(position);
			} //if

			return 0;
		} //getZoom
		
		void setZoom(const float& zoom)
		{
			if (target)
			{
				if (zoom > 0)
				{
					if (position.x != 0 || position.y != 0 || position.z != 0)
					{
						position = glm::normalize(position);
					} //if
					else
					{
						position = orientation*glm::vec3(0, 0, -1);
					} //else
					
					position.x *= zoom;
					position.y *= zoom;
					position.z *= zoom;
				} //if
				else
				{
					position.x = 0;
					position.y = 0;
					position.z = 0;
					
					orientation.x = 0;
					orientation.y = 0;
					orientation.z = 0;
					orientation.w = 1;
					
					orientationX = orientation;
					orientationY = orientation;
				} //else
			} //if
		} //setZoom
		
		void zoom(const float& zoom)
		{
			setZoom(getZoom() + zoom);
		} //zoom
		
		void rotate(const glm::vec3& rotation)
		{
			//yaw = {0, 1, 0}
			//pitch  = {1, 0, 0}
			//roll = {0, 0, 1

			if (target && getZoom() == 0)
			{
				//rotate target
			} //if
			else
			{
				/*
				glm::vec3 forward(glm::vec3(0, 0, 1)*orientation);
				forward.y = 0;
				forward = glm::normalize(forward);
				glm::vec3 side = glm::cross(up, forward);
				
				orientation = glm::rotate(orientation, rotation.x, side);
				orientation = glm::rotate(orientation, rotation.z, forward);
				orientation = glm::rotate(orientation, rotation.y, up);
				*/
				
				orientationX = glm::rotate(orientationX, rotation.x, glm::vec3(1, 0, 0));
				orientationY = glm::rotate(orientationY, glm::pitch(orientationX) > glm::pi<float>()/2 || glm::pitch(orientationX) < -glm::pi<float>()/2? -rotation.y : rotation.y, up);
				orientation = orientationX*orientationY;
			} //else
		} //rotate
		
		void setRotation(const glm::quat& orientation)
		{
			this->orientation = orientation;
			orientationX = glm::angleAxis(glm::pitch(orientation), glm::vec3(1, 0, 0));
			orientationY = glm::angleAxis(glm::yaw(orientation), up);
		} //setRotation
		
		glm::vec3 getRotation()
		{
			return orientation*glm::vec3(0, 0, -1);
		} //getRotation
		
		//theta, phi, zoom
		//x, y, z
		void setPosition(const glm::vec3& position)
		{
			if (target)
			{
				setZoom(position.z);
			} //if
			else
			{
				this->position = position;
			} //else
		} //setPosition
		
		//x, y, zoom
		//x, y, z
		void translate(const glm::vec3& translation)
		{
			if (target)
			{
				setZoom(getZoom() + translation.z);
				//rotate(glm::vec3(translation.x, translation.y, 0));
			} //if
			else
			{
				position += translation*orientation;
			} //else
		} //translate
		
		void setTransformation(const glm::mat4& transformation)
		{
			position = glm::vec3(transformation[3][0], transformation[3][1], transformation[3][2]);
			setRotation(glm::toQuat(transformation));
		} //setTransformation
}; //Camera
