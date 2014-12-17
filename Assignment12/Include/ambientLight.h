#pragma once

#include <glm/common.hpp>

class AmbientLight
{
public:
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;

	void copyTo(AmbientLight& ambientLight)
	{
		ambientLight.color = color;
		ambientLight.ambientIntensity = ambientIntensity;
		ambientLight.diffuseIntensity = diffuseIntensity;
	} //copyTo
}; //AmbientLight