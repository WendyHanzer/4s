#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>

struct Vertex
{
	GLfloat position[3];
	GLfloat texture[2];
	GLfloat color[4];
	GLfloat normal[3];
}; //Vertex
