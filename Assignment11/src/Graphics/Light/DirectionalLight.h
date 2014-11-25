#pragma once

#include "Light.h"

struct DirectionalLight : public Light
{
	DirectionalLight() : Light(0, "DirectionalLight") { }
	DirectionalLight(const uint64_t& ID, const std::string& NAME) : Light(ID, NAME) { }
	
	glm::vec3 direction;
	
	virtual void load(ShaderProgram& program, const int& index)
	{
		std::string pos = std::string("directionalLight[") + std::to_string(index) + std::string("].");
		
		glUniform3f(program.uniform(pos + "base.color"), color.x, color.y, color.z);
		glUniform1f(program.uniform(pos + "base.ambientIntensity"), ambientIntensity);
		glUniform1f(program.uniform(pos + "base.diffuseIntensity"), diffuseIntensity);
		
		glm::vec3 nDirection = glm::normalize(direction);
		
		glUniform3f(program.uniform(pos + "direction"), nDirection.x, nDirection.y, nDirection.z);
	} //load
}; //DirectionalLight
