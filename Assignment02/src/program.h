#pragma once

#include <iostream>

#include "shader.h"

class Program
{
	struct Location
	{
		GLint position;
		GLint color;
		GLint mvpmat;
	};

public:
	GLuint program;
	Location location;
	
	bool loadShader(Shader& shader)
	{
		GLint shaderStatus;
	
		glAttachShader(program, shader.vertexShader);
		glAttachShader(program, shader.fragmentShader);
		glLinkProgram(program);
		
		//check if everything linked ok
		glGetProgramiv(program, GL_LINK_STATUS, &shaderStatus);
		if(!shaderStatus)
		{
		    std::cerr << "[F] THE SHADER PROGRAM FAILED TO LINK" << std::endl;
		    return false;
		}
		
		//Now we set the locations of the attributes and uniforms
		//this allows us to access them easily while rendering
		location.position = glGetAttribLocation(program, "v_position");
		if(location.position == -1)
		{
		    std::cerr << "[F] POSITION NOT FOUND" << std::endl;
		    return false;
		}

		location.color = glGetAttribLocation(program, "v_color");
		if(location.color == -1)
		{
		    std::cerr << "[F] V_COLOR NOT FOUND" << std::endl;
		    return false;
		}

		location.mvpmat = glGetUniformLocation(program, "mvpMatrix");
		if(location.mvpmat == -1)
		{
		    std::cerr << "[F] MVPMATRIX NOT FOUND" << std::endl;
		    return false;
		}
		
		return true;
	} //loadShader
	
	Program()
	{
		program = glCreateProgram();
	} //constructor
	
	~Program()
	{
		glDeleteProgram(program);
	} //destructor
};
