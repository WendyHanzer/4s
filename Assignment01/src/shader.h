#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <fstream>

using namespace std;

/***************************************************************************
*
*	Shader
*
*		This class is used to load vertex and fragment shaders to a program.
*
*		It will handle
*
*			1.	Reading from file or string
*			2.	Compilation
*			3.	Linking
*
****************************************************************************
*
*	Settings
*/
	#define SHADER__DEFAULT_VERTEX_SHADER_FILENAME "vs"
	#define SHADER__DEFAULT_FRAGMENT_SHADER_FILENAME "fs"
/*
***************************************************************************
*
*	API
*
*		Constructor
*		--------------------------
*
*			Shader()
*				-	Creates one empty vertex shader and one empty fragment shader for later use.
*
*				-	Does not take a program to enable a given Shader class to be applied to multiple
*					programs if desired.
*
*		Methods
*		--------------------------
*
*			bool loadShadersFile(const char* vertexShaderFilename, const char* fragmentShaderFilename, GLuint& program)
*				-	This will take two file names. When no file name is supplied, a default file name is used instead
*					(see settings).
*
*				-	The data from the files will be loaded, compiled, and linked. If any errors occur, an error message
*					will be displayed and the method will return false.
*
*			bool loadShaders(const char* vs, const char* fs, GLuint& program)
*				-	This will take a vertex shader script, a fragment shader script, and a program.
*
*				-	The two scripts will be compiled and linked. If any errors occur, an error message
*					will be displayed and the method will return false.
*
*		Auxiliary Methods
*		--------------------------
*
*			These methods are used when wanting to replace either the vertex shader
*			or fragment shader alone.
*
*			bool loadVertexShaderFile(const char* filename)
*				-	This will load and compile a vertex shader from a file given that file's name.
*
*				-	If any errors occur, an error message will be displayed and the method will
*					return false.
*
*			bool loadVertexShader(const char* vs)
*				-	This will compile a vertex shader script.
*
*				-	If any errors occur, an error message will be displayed and the method will
*					return false.
*
*
*			bool loadFragmentShaderFile(const char* filename)
*				-	This will load and compile a fragment shader from a file given that file's name.
*
*				-	If any errors occur, an error message will be displayed and the method will
*					return false.
*
*			bool loadFragmentShader(const char* fs)
*				-	This will compile a fragment shader script.
*
*				-	If any errors occur, an error message will be displayed and the method will
*					return false.
*
*
*			bool linkShaders(GLuint& program)
*				-	This will attach the vertex shader and fragment shader to the supplied
*					program and then link them.
*
*				-	If any errors occur, an error message will be displayed and the method will
*					return false.
*
***************************************************************************/
class Shader
{
private:
	GLuint vertex_shader;
    GLuint fragment_shader;
    
	static char* loadFile(const char* filename)
	{
		char* fileData;
		long fileSize;
		
		fstream file(filename, ios::in|ios::binary|ios::ate);
		
		if (!file.is_open())
		{
			return 0;
		} //if
		
		fileSize = file.tellg();
		file.seekg(0, ios::beg);
		fileData = new char[fileSize];
		file.read(fileData, fileSize);
		file.close();
		
		return fileData;
	}
	
public:
	bool loadVertexShaderFile(const char* filename)
	{
		char* vs;
		bool success;
		
		/*
		*	use if here to allow use of const char*
		*/
		if (filename == 0)
		{
			vs = loadFile(SHADER__DEFAULT_VERTEX_SHADER_FILENAME);
		} //if
		else
		{
			vs = loadFile(filename);
		} //else
		
		success = loadVertexShader(vs);
		
		if (vs != 0)
		{
			delete[] vs;
		} //if
		
		return success;
	} //loadVertexShaderFile
	
	bool loadVertexShader(const char* vs)
	{
		GLint shader_status;
	
		/*
		*	check for vertex shader
		*/
		if (vs == 0)
		{
			std::cerr << "[F] NO VERTEX SHADER DATA WAS SUPPLIED!" << std::endl;
			return false;
		}
		
		/*
		*	compile vertex shader
		*/
		glShaderSource(vertex_shader, 1, &vs, NULL);
		glCompileShader(vertex_shader);
		
		/*
		*	check compilation
		*/
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_status);
		if(!shader_status)
		{
		    std::cerr << "[F] FAILED TO COMPILE VERTEX SHADER!" << std::endl;
		    return false;
		}
		
		return true;
	}
	
	bool loadFragmentShaderFile(const char* filename)
	{
		char* fs;
		bool success;
		
		/*
		*	use if here to allow use of const char*
		*/
		if (filename == 0)
		{
			fs = loadFile(SHADER__DEFAULT_FRAGMENT_SHADER_FILENAME);
		} //if
		else
		{
			fs = loadFile(filename);
		} //else
		
		success = loadFragmentShader(fs);
		
		if (fs != 0)
		{
			delete[] fs;
		} //if
		
		return success;
	} //loadFragmentShaderFile
	
	bool loadFragmentShader(const char* fs)
	{
		GLint shader_status;
	
		/*
		*	check for fragment shader
		*/
		if (fs == 0)
		{
			std::cerr << "[F] NO FRAGMENT SHADER DATA WAS SUPPLIED!" << std::endl;
			return false;
		}
	
		/*
		*	compile fragment shader
		*/
		glShaderSource(fragment_shader, 1, &fs, NULL);
		glCompileShader(fragment_shader);
		
		/*
		*	check compilation
		*/
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_status);
		if(!shader_status)
		{
		    std::cerr << "[F] FAILED TO COMPILE FRAGMENT SHADER!" << std::endl;
		    return false;
		}
		
		return true;
	} //loadFragmentShader
	
	bool linkShaders(GLuint& program)
	{
		GLint shader_status;
	
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);
		
		//check if everything linked ok
		glGetProgramiv(program, GL_LINK_STATUS, &shader_status);
		if(!shader_status)
		{
		    std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
		    return false;
		}
		
		return true;
	} //linkShaders
	
	bool loadShadersFile(const char* vertexShaderFilename, const char* fragmentShaderFilename, GLuint& program)
	{
		return loadVertexShaderFile(vertexShaderFilename) && loadFragmentShaderFile(fragmentShaderFilename) && linkShaders(program);
	} //loadShadersFile
	
	bool loadShaders(const char* vs, const char* fs, GLuint& program)
	{
		return loadVertexShader(vs) && loadFragmentShader(fs) && linkShaders(program);
	} //loadShaders

	Shader()
	{
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	} //Shader constructor
};
#undef SHADER__DEFAULT_VERTEX_SHADER_FILENAME
#undef SHADER__DEFAULT_FRAGMENT_SHADER_FILENAME
