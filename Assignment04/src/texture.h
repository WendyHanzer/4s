#pragma once

#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>

/*
*	work in progress
*/

class Texture
{
public:
	GLenum textureTarget;
	GLuint textureObject;
	Magick::Image* image;
	Magick::Blob blob;
	
	Texture(GLenum textureTarget, const char* filename)
	{
		textureTarget = textureTarget;
		image = NULL;
		
		cout << "Loading image: " << filename << endl;
		
		try
		{
			image = new Magick::Image(filename);
			image->write(&blob, "RGBA");
		} //try
		catch (Magick::Error& exception)
		{
			image = NULL;
			std::cerr << "[F] TEXTURE LOAD ERROR " << filename << ": " << exception.what() << std::endl;
			return;
		}

		glGenTextures(1, &textureObject);
		glBindTexture(textureTarget, textureObject);
		glTexImage2D(textureTarget, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	~Texture()
	{
		glDeleteTextures(1, &textureObject);
	}

	void bind(GLenum textureUnit)
	{
		glActiveTexture(textureUnit);
		glBindTexture(textureTarget, textureObject);
	} //bind
};
