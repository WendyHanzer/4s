#include <iostream>

#include "shaderProgram.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

// Constructor
ShaderProgram::ShaderProgram()
{
	// Generate a unique Id / handle for the shader program
	// Note: We MUST have a valid rendering context before generating
	// the programId or it causes a segfault!
	programId = glCreateProgram();

	// Initially, we have zero shaders attached to the program
	shaderCount = 0;

	linked = false;
} //ShaderProgram

// Destructor
ShaderProgram::~ShaderProgram()
{
	// Delete the shader program from the graphics card memory to
	// free all the resources it's been using
	glDeleteProgram(programId);
} //~ShaderProgram

bool ShaderProgram::getBoolParameter(GLenum pname)
{
	GLint ret;
	glGetProgramiv(programId, pname, &ret);
	return ret == GL_TRUE;
} //getBoolParameter

GLint ShaderProgram::getIntParameter(GLenum pname)
{
	GLint ret;
	glGetProgramiv(programId, pname, &ret);
	return ret;
} //getIntParameter

// Method to attach a shader to the shader program
void ShaderProgram::attachShader(Shader& shader)
{
	if (linked)
	{
		return;
	} //if

	// Attach the shader to the program
	// Note: We identify the shader by its unique Id value
	glAttachShader(programId, shader.getId());

	// Increment the number of shaders we have associated with the program
	++shaderCount;
} //attachShader

bool ShaderProgram::validate()
{
	if (!linked)
	{
		return false;
	} //if

	bool validated = isValidated();

	if (!validated)
	{
		GLint infoLogLength = getInfoLogLength();

		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(programId, infoLogLength, NULL, strInfoLog);

		cerr << "Invalid shader program: " << strInfoLog << endl;

		delete[] strInfoLog;
	} //if

	return validated;
} //validate

// Method to link the shader program and display the link status
bool ShaderProgram::link()
{
	if (linked)
	{
		return false;
	} //if

	// If we have at least two shaders (like a vertex shader and a fragment shader)...
	if (shaderCount > 1)
	{
		// Perform the linking process
		glLinkProgram(programId);

		// Check the status
		GLint linkStatus;
		glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
		if (GL_LINK_STATUS == GL_FALSE)
		{
			cerr << "Shader program linking failed." << endl;
			linked = false;
		} //if
		else
		{
			cout << "Shader program linking OK." << endl;
			linked = true;
		} //else
	} //if
	else
	{
		cerr << "Can't link shaders - you need at least 2, but attached shader count is only: " << shaderCount << endl;
		linked = false;
	} //else

	return linked;
} //link