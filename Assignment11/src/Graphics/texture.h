#pragma once

#include <GL/glew.h>
#include <ImageMagick-6/Magick++.h>

struct Texture
{
	GLenum textureTarget;
	GLuint textureObject;
	
public:

	bool loaded;
	
	Texture(const char* filename, GLenum textureTarget = GL_TEXTURE_2D)
	{
		Magick::Image* image;
		Magick::Blob* blob;
	
		loaded = false;
	
		textureObject = -1;
		
		this->textureTarget = textureTarget;
		
		cout << "Loading image: " << filename << endl;
		
		try
		{
			image = new Magick::Image(filename);
		} //try
		catch (Magick::Error& exception)
		{
			std::cerr << "[F] TEXTURE LOAD ERROR " << filename << ": " << exception.what() << std::endl;
			return;
		}
		
		blob = new Magick::Blob();
		image->magick("RGBA");
		image->write(blob);
		
		glGenTextures(1, &textureObject);
		
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(textureTarget, textureObject);
		
		glTexImage2D(textureTarget, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob->data());
		
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glBindTexture(textureTarget, 0);
		
		delete image;
		delete blob;
		
		loaded = true;
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
	
	void unbind(GLenum textureUnit)
	{
		glActiveTexture(textureUnit);
		glBindTexture(textureTarget, 0);
	} //unbind
};
