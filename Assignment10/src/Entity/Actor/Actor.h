#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "../../Engine/Entity/Entity.h"
#include "ActorManager.h"

class Actor : public Entity
{
	private:

		glm::mat4 transformation;

	public:
		Event<void> eventTransformationChange;

		Actor(ActorManager& actorManager, const std::string name);
		virtual ~Actor();

		void setTransformation(const glm::mat4& transformation);

		inline const glm::mat4& getTransformation() const
		{
			return transformation;
		} //getTransformation
}; //Actor
