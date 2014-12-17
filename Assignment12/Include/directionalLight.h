#pragma once

#include "ambientLight.h"

class DirectionalLight : public AmbientLight
{
public:
	glm::vec3 direction;

	void copyTo(DirectionalLight& directionalLight)
	{
		AmbientLight::copyTo(directionalLight);
		directionalLight.direction = direction;
	} //copyTo
}; //DirectionalLight