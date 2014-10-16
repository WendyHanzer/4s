#pragma once

#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/*
*	work in progress
*/

struct Texture
{
	GLenum textureTarget;
	GLuint textureObject;

public:


	bool loaded;
	/*
	*	Constructors
	*/
	Texture(GLenum textureTarget, const char* filename);
	~Texture();

	/*
	*	Bind Operation
	*/
	void bind(GLenum textureUnit);
	void unbind(GLenum textureUnit);
}; //Texture
