#pragma once

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>

struct Vertex
{
	GLfloat position[3];	//24 	bytes
	GLushort texture[2];	//4 	bytes
	GLfloat color[4];		//32	bytes
	GLuint normal[3];		//24	bytes
							//-----------
							//84	bytes
};
