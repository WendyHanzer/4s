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

#include <stdio.h>
#include <string.h>

#include "ogldev_util.h"
#include "technique.h"

Technique::Technique()
{
	m_shaderProg = program.getId();
}


bool Technique::Init()
{
	return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Technique::AddShader(GLenum ShaderType, const char* pFilename)
{
	Shader shader(ShaderType);

	shader.loadFromFile(pFilename);

	if (shader.compile())
	{
		program.attachShader(shader);
	} //if

	return shader.isCompiled();
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Technique::Finalize()
{
	return program.link() && program.validate();
}


void Technique::Enable()
{
	program.bind();
}


GLint Technique::GetUniformLocation(const char* pUniformName)
{
	return program.uniformLocation(pUniformName);
}

GLint Technique::GetProgramParam(GLint param)
{
	return program.getIntParameter(param);
}
