#include "Texture.h"

Texture::Texture(GLenum textureTarget, const char* filename)
{
	Magick::Image* image;
	Magick::Blob* blob;

	loaded = false;

	Texture::textureObject = -1;
	
	this->Texture::textureTarget = Texture::textureTarget;
	
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
	glBindTexture(Texture::textureTarget, Texture::textureObject);
	
	glTexImage2D(Texture::textureTarget, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob->data());
	
	glTexParameteri(Texture::textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(Texture::textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(Texture::textureTarget, 0);
	
	delete image;
	delete blob;
	
	loaded = true;
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureObject);
}

void Texture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(Texture::textureTarget, Texture::textureObject);
} //bind

void Texture::unbind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(Texture::textureTarget, 0);
} //unbind
