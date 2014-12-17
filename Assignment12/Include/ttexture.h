#pragma once

#include <string>
#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>

struct TTexture
{
private:
	GLenum textureTarget;
	GLuint textureObject;
	GLenum textureUnit;

	bool loaded;
	bool bound;

public:

	TTexture(GLenum textureTarget = GL_TEXTURE_2D);
	~TTexture();

	void setTexture(const std::string& filename);

	void bind(GLenum textureUnit);
	void unbind();

	inline bool isLoaded() { return loaded; }
	inline bool isBound() { return bound; }
}; //Texture