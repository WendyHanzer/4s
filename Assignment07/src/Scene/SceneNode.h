#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion>

#include <list>

#include "../EngineObjects/MoveableObject.h"

class MoveableObject;

class SceneNode
{
	private:
		SceneNode* parent;

		glm::mat4 transformation;

		std::list<SceneNode> children;
		std::list<MoveableObject> objects;

	public:
		/*
		*	Lookat Operation
		*/
		void lookAt(glm::vec3 point);
		void lookAt(SceneNode* node);

		void rotate(glm::vec3 rotation);
		void translate(glm::vec3 translation);
		void scale(glm::vec3 scale);

		void setRotation(glm::vec3 rotation);
		void setOrientation(glm::quat orientation);
		void setPosition(glm::vec3 position);
		void setScale(glm::vec3 scale);

		glm::quat getOrientation();
		glm::vec3 getPosition();
		glm::vec3 getScale();

		glm::quat getWorldOrientation();
		glm::vec3 getWorldPosition();
		glm::vec3 getWorldScale();

		void attach(MoveableObject* object);
		void detach(MoveableObject* object);
}; //SceneNode