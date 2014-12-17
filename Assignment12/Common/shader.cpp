#pragma once

#include <iostream>
#include <sstream>
#include <fstream>

#include "shader.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::ifstream;

// Constructor
Shader::Shader(const GLenum type)
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
		typeString = "Vertex";
		break;
	case GL_FRAGMENT_SHADER:
		typeString = "Fragment";
		break;
	case GL_GEOMETRY_SHADER:
		typeString = "Geometry";
		break;
	default:
		typeString = "";
		break;
	} //switch

	// Create the vertex shader id / handle
	// Note: If you segfault here you probably don't have a valid rendering context.
	id = glCreateShader(type);

	compiled = false;
	loaded = false;
} //Shader

Shader::~Shader()
{
	glDeleteShader(id);
} //~Shader

// Method to load the shader contents from a string
void Shader::loadFromString(const string& sourceString)
{
	// Keep hold of a copy of the source
	source = sourceString;

	// Get the source as a pointer to an array of characters
	const char *sourceChars = source.c_str();

	// Associate the source with the shader id
	glShaderSource(id, 1, &sourceChars, NULL);

	loaded = true;
} //loadFromString

// Method to load the shader contents from a file
bool Shader::loadFromFile(const string& filename)
{
	ifstream file;

	file.open(filename.c_str());

	if (!file.good())
	{
		cout << "Failed to open file: " << filename << endl;

		loaded = false;
		return false;
	} //if

	// Create a string stream
	stringstream stream;

	// Dump the contents of the file into it
	stream << file.rdbuf();

	// Close the file
	file.close();

	loadFromString(stream.str());

	return true;
} //loadFromFile

// Method to compile a shader and display any problems if compilation fails
bool Shader::compile()
{
	if (!loaded)
	{
		return false;
	} //if

	// Compile the shader
	glCompileShader(id);

	// Check the compilation status and report any errors
	GLint shaderStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &shaderStatus);

	compiled = shaderStatus != GL_FALSE;

	if (compiled)
	{
		cout << typeString << " shader compilation OK" << endl;
	} //if
	else
	{
		GLint infoLogLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(id, infoLogLength, NULL, strInfoLog);

		cout << typeString << " shader compilation failed: " << strInfoLog << endl;
		delete[] strInfoLog;
	} //else

	return compiled;
} //compile