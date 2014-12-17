/*

	Copyright 2011 Etay Meiri

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	*/

#include <iostream>
#include "ogldev_texture.h"

Texture::Texture(GLenum TextureTarget, const std::string& FileName) : texture(TextureTarget)
{
	m_fileName = FileName;
}

bool Texture::Load()
{
	texture.setTexture(m_fileName);

	return texture.isLoaded();
}

void Texture::Bind(GLenum TextureUnit)
{
	texture.bind(TextureUnit);
}
