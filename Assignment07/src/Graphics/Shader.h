#pragma once
 
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <GL/glew.h>
 
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::ifstream;

//http://r3dux.org/2013/08/a-simple-c-opengl-shader-loader/
 
class Shader
{
	private:
		GLuint id; // The unique ID / handle for the shader
		string typeString; // String representation of the shader type (i.e. "Vertex" or such)
		string source; // The shader source code (i.e. the GLSL code itself)
	 
	public:
		/*
		*	Constructors
		*/	
		Shader(const GLenum type);
		/*
		*	Get Operations
		*/
		GLuint getId();
		string getSource();
		/*
		*	Load Operations
		*/
		// Method to load the shader contents from a string
		void loadFromString(const string sourceString);
		// Method to load the shader contents from a file
		void loadFromFile(const string filename);
		 
		/*
		*	Compile
		*/
		// Method to compile a shader and display any problems if compilation fails
		void compile();
};
