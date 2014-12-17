#include "ttexture.h"

#include <iostream>

TTexture::TTexture(GLenum textureTarget)
{
	this->textureTarget = textureTarget;

	loaded = false;
	bound = false;
} //Texture

TTexture::~TTexture()
{
	if (loaded)
	{
		if (bound)
		{
			unbind();
		} //if

		glDeleteTextures(1, &textureObject);
	} //if
} //~Texture

void TTexture::setTexture(const std::string& filename)
{
	if (loaded)
	{
		glDeleteTextures(1, &textureObject);

		loaded = false;
		bound = false;
	} //if

	Magick::Image image;
	Magick::Blob blob;

	try
	{
		image.read(filename);
		image.write(&blob, "RGBA");
	} //try
	catch (Magick::Error& exception)
	{
		std::cerr << "Error loading texture '" << filename << "': " << exception.what() << std::endl;
		return;
	} //catch

	glGenTextures(1, &textureObject);

	glBindTexture(textureTarget, textureObject);

	glTexImage2D(textureTarget, 0, GL_RGBA, image.columns(), image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());

	glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(textureTarget, 0);

	loaded = true;
} //setTexture

void TTexture::bind(GLenum textureUnit)
{
	if (loaded)
	{
		glActiveTexture(textureUnit);
		glBindTexture(textureTarget, textureObject);
		glActiveTexture(0);

		this->textureUnit = textureUnit;
		bound = true;
	} //if
} //bind

void TTexture::unbind()
{
	if (bound)
	{
		glActiveTexture(textureUnit);
		glBindTexture(textureTarget, 0);
		glActiveTexture(0);

		bound = false;
	} //if
} //unbind