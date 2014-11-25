#pragma once

#define GLM_FORCE_RADIANS

#include <stdint.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <string>
#include <map>
#include <vector>

#include "../shaderProgram.h"

struct Light
{
	const uint64_t ID;
	const std::string NAME;

	glm::vec3 color;
	GLfloat diffuseIntensity;
	GLfloat ambientIntensity;
	
	Light(const uint64_t& ID, const std::string& NAME) : ID(ID), NAME(NAME) { }
	
	virtual void load(ShaderProgram& program, const int& index) { }
	
	virtual ~Light() { }
}; //Light
