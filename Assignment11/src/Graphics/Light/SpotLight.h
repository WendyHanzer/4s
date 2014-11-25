#pragma once

#include <cmath>

#include "PointLight.h"

struct SpotLight : public PointLight
{
	glm::vec3 direction;
	float cutoff;				//angle in radians
	
	SpotLight() : PointLight(2, "SpotLight") { }
	SpotLight(const uint64_t& ID, const std::string& NAME) : PointLight(ID, NAME) { }
	
	virtual void load(ShaderProgram& program, const int& index)
	{
		std::string pos = std::string("spotLight[") + std::to_string(index) + std::string("].");
		
		glUniform3f(program.uniform(pos + "base.base.color"), color.x, color.y, color.z);
		glUniform1f(program.uniform(pos + "base.base.ambientIntensity"), ambientIntensity);
		glUniform1f(program.uniform(pos + "base.base.diffuseIntensity"), diffuseIntensity);
		
		glUniform3f(program.uniform(pos + "base.position"), position.x, position.y, position.z);
		glUniform1f(program.uniform(pos + "base.atten.constant"), attenuation.constant);
		glUniform1f(program.uniform(pos + "base.atten.linear"), attenuation.linear);
		glUniform1f(program.uniform(pos + "base.atten.exp"), attenuation.exp);
		
		glm::vec3 nDirection = glm::normalize(direction);
		glUniform3f(program.uniform(pos + "direction"), nDirection.x, nDirection.y, nDirection.z);
		
		glUniform1f(program.uniform(pos + "cutoff"), cos(cutoff));
	} //load
}; //SpotLight
