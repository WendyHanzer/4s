#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cmath>

glm::vec3 getForwardVector(const glm::quat& orientation);
glm::vec3 getUpVector(const glm::quat& orientation);
glm::vec3 getRightVector(const glm::quat& orientation);
glm::quat quatBetweenVectors(const glm::vec3& start, const glm::vec3& dest);
glm::quat vectorToQuaternion(const glm::vec3& direction);
