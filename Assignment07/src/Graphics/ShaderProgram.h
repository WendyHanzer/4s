#pragma once
 
#include <iostream>
#include <map>
 
#include "Shader.h"
 
using std::map;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

//http://r3dux.org/2013/08/a-simple-c-opengl-shader-loader/

class ShaderProgram
{
	private:
		GLuint programId; 	// The unique ID / handle for the shader program
		GLuint shaderCount; // How many shaders are attached to the shader program
		 
		// Map of attributes and their binding locations
		map<string,int> attributeLocList;
		 
		// Map of uniforms and their binding locations
		map<string,int> uniformLocList;
	 
	public:
		/*
		*	Constructors
		*/
		ShaderProgram();
		~ShaderProgram();

		/*
		*	Shader Operation
		*/
		// Method to attach a shader to the shader program
		void attachShader(Shader shader);	 
		// Method to link the shader program and display the link status
		void linkProgram();
		// Method to enable the shader program
		void use();
		// Method to disable the shader program
		void disable();
		// Method to add an attrbute to the shader and return the bound location
		int addAttribute(const string attributeName); 
		// Method to add a uniform to the shader and return the bound location
		int addUniform(const string uniformName);

		/*
		*	Get Operation
		*/
		// Returns the bound location of a named attribute
		GLuint attribute(const string attribute);
		// Method to returns the bound location of a named uniform
		GLuint uniform(const string uniform);
};
