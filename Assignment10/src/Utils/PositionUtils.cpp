#include "PositionUtils.h"

glm::vec3 getForwardVector(const glm::quat& orientation)
{
	return glm::vec3(	2*(orientation.x*orientation.z + orientation.w*orientation.y),
						2*(orientation.y*orientation.z - orientation.w*orientation.x),
						1 - 2*(orientation.x*orientation.x + orientation.y*orientation.y)
					);
} //getForwardVector

glm::vec3 getUpVector(const glm::quat& orientation)
{
	return glm::vec3(	2*(orientation.x*orientation.y - orientation.w*orientation.z),
						1 - 2*(orientation.x*orientation.x + orientation.z*orientation.z),
						2*(orientation.y*orientation.z + orientation.w*orientation.x)
					);
} //getUpVector

glm::vec3 getRightVector(const glm::quat& orientation)
{
	return glm::vec3(	1 - 2*(orientation.y*orientation.y + orientation.z*orientation.z),
						2*(orientation.x*orientation.y + orientation.w*orientation.z),
						2*(orientation.x*orientation.z - orientation.w*orientation.y)
					);
} //getRightVector

glm::quat vectorToQuaternion(const glm::vec3& direction)
{
	//float theta = atan2(-direction.z, direction.x);
	//float phi = atan2(glm::sqrt(direction.x*direction.x + direction.y*direction.y), direction.z);

	//glm::quat qx(glm::angleAxis(theta, glm::vec3(0, 1, 0)));
	//glm::quat qy(glm::angleAxis(phi, glm::vec3(1, 0, 0)));
	
	//return qx;

	glm::vec3 v = glm::normalize(direction);

	glm::vec3 up(0, 1, 0);
	glm::vec3 right = glm::normalize(glm::cross(up, v));
	up = glm::normalize(glm::cross(v, right));
	
	glm::mat4 basis(
		right.x, right.y, right.z, 0.0f,
		up.x, up.y, up.z, 0.0f,
		v.x, v.y, v.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
		
	return glm::toQuat(basis);
} //vectorToQuaternion


glm::quat quatBetweenVectors(const glm::vec3& start, const glm::vec3& dest)
{
	glm::vec3 dif = dest - start;
	
	if (glm::length2(dif) == 0)
	{
		return glm::quat(0, 0, 0, 1);
	} //if
	
	return vectorToQuaternion(dif);
} //quatBetweenVectors
