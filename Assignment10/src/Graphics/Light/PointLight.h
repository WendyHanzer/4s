#pragma once

#include "Light.h"

struct Attenuation
{
	float constant;
	float linear;
	float exp;
}; //Attenuation

struct PointLight : public Light
{
	glm::vec3 position;
	Attenuation attenuation;
	
	PointLight() : Light(1, "PointLight") { }
	PointLight(const uint64_t& ID, const std::string& NAME) : Light(ID, NAME) { }
	
	virtual void load(ShaderProgram& program, const int& index)
	{
		std::string pos = std::string("pointLight[") + std::to_string(index) + std::string("].");
		
		glUniform3f(program.uniform(pos + "base.color"), color.x, color.y, color.z);
		glUniform1f(program.uniform(pos + "base.ambientIntensity"), ambientIntensity);
		glUniform1f(program.uniform(pos + "base.diffuseIntensity"), diffuseIntensity);
		glUniform3f(program.uniform(pos + "position"), position.x, position.y, position.z);
		glUniform1f(program.uniform(pos + "atten.constant"), attenuation.constant);
		glUniform1f(program.uniform(pos + "atten.linear"), attenuation.linear);
		glUniform1f(program.uniform(pos + "atten.exp"), attenuation.exp);
	} //load
}; //PointLight
