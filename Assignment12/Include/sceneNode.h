#pragma once

#define GLM_FORCE_RADIANS
#include <glm/common.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <vector>

#include "indexedArray.h"
#include "sceneFrame.h"

class SceneNodePosition
{
public:
	glm::mat4 worldTransformation;
	glm::vec3 modelPosition;
	glm::vec3 modelScale;
	glm::quat modelRotation;

	void copyTo(SceneNodePosition& position)
	{
		position.worldTransformation = worldTransformation;
		position.modelPosition = modelPosition;
		position.modelScale = modelScale;
		position.modelRotation = modelRotation;
	} //copyTo
}; //SceneNodePosition

class SceneNode
{
protected:
	virtual void onVisit(SceneFrame& frame, glm::mat4& position) { }

	void copyTo(SceneNodePosition& position)
	{
		position.worldTransformation = worldPosition;
		position.modelPosition = this->position;
		position.modelScale = this->scale;
		position.modelRotation = this->rotation;
	} //copyTo

public:
	glm::mat4 worldPosition;

	std::vector<SceneNode*> children;

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	inline const glm::vec3 getForwardVector() { return rotation*glm::vec3(0, 0, -1); }
	inline const glm::vec3 getRightVector() { return rotation*glm::vec3(1, 0, 0); }
	inline const glm::vec3 getUpVector() { return rotation*glm::vec3(0, 1, 0); }
	inline void setRotation(const glm::vec3 direction)
	{
		//glm::quat base = glm::rotation(glm::vec3(0, 0, -1), getForwardVector());
		//glm::quat rollQuat = glm::rotation(base*glm::vec3(0, 1, 0), getUpVector());
		//float roll = glm::angle(rollQuat);

		//dunno how to maintain roll
		glm::quat q = glm::rotation(glm::vec3(0, 0, -1), direction);
		//glm::quat q2 = glm::rotate(q, roll, q*glm::vec3(0, 1, 0));

		rotation = q;
	}
	inline void rotate(float angle, glm::vec3 axis) { rotation = glm::rotate(rotation, angle, axis); }
	inline void setRotationEuler(const glm::vec3 direction) { rotation = glm::toQuat(glm::eulerAngleYXZ(direction.y, direction.x, direction.z)); }
	inline glm::mat4 modelCoordinate() { return glm::translate(glm::toMat4(rotation) * glm::scale(glm::mat4(), scale), position); }
	inline glm::mat4 worldCoordinate(const glm::mat4& parent) { return parent*modelCoordinate(); }
	inline void lookAt(const glm::vec3& position) { setRotation(position - this->position); }

	void visit(SceneFrame& frame, const glm::mat4& position = glm::mat4())
	{
		worldPosition = worldCoordinate(position);
		onVisit(frame, worldPosition);

		for (SceneNode* child : children)
		{
			child->visit(frame, worldPosition);
		} //for
	} //visit
}; //SceneNode