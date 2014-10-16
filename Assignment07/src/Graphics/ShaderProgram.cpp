#include "ShaderProgram.h"
// Constructor
ShaderProgram::ShaderProgram()
{
	// Generate a unique Id / handle for the shader program
	// Note: We MUST have a valid rendering context before generating
	// the programId or it causes a segfault!
	ShaderProgram::programId = glCreateProgram();
	 
	// Initially, we have zero shaders attached to the program
	ShaderProgram::shaderCount = 0;
}
 
 
// Destructor
ShaderProgram::~ShaderProgram()
{
	// Delete the shader program from the graphics card memory to
	// free all the resources it's been using
	glDeleteProgram(ShaderProgram::programId);
}
 
 
// Method to attach a shader to the shader program
void ShaderProgram::attachShader(Shader shader)
{
	// Attach the shader to the program
	// Note: We identify the shader by its unique Id value
	glAttachShader(ShaderProgram::programId, shader.getId());
	 
	// Increment the number of shaders we have associated with the program
	++ShaderProgram::shaderCount;
}
 
 
// Method to link the shader program and display the link status
void ShaderProgram::linkProgram()
{
	// If we have at least two shaders (like a vertex shader and a fragment shader)...
	if (ShaderProgram::shaderCount >= 2)
	{
		// Perform the linking process
		glLinkProgram(ShaderProgram::programId);
		 
		// Check the status
		GLint linkStatus;
		glGetProgramiv(ShaderProgram::programId, GL_LINK_STATUS, &linkStatus);
		if (GL_LINK_STATUS == GL_FALSE)
		{
			cerr << "Shader program linking failed." << endl;
			exit(-1);
		}
		else
		{
			cout << "Shader program linking OK." << endl;
		}
	}
	else
	{
		cerr << "Can't link shaders - you need at least 2, but attached shader count is only: " << ShaderProgram::shaderCount << endl;
		exit(-1);
	}
}
 
 
// Method to enable the shader program
void ShaderProgram::use()
{
	glUseProgram(ShaderProgram::programId);
}
 
 
// Method to disable the shader program
void ShaderProgram::disable()
{
	glUseProgram(0);
}
 
 
// Returns the bound location of a named attribute
GLuint ShaderProgram::attribute(const string attribute)
{
	// You could do this function with the single line:
	//
	//		return attributeLocList[attribute];
	//
	// BUT, if you did, and you asked it for a named attribute
	// which didn't exist, like, attributeLocList["ThisAttribDoesn'tExist!"]
	// then the method would return an invalid value which will likely cause
	// the program to segfault. So we're making sure the attribute asked
	// for exists, and if it doesn't we can alert the user and stop rather than bombing out later.
	 
	// Create an iterator to look through our attribute map and try to find the named attribute
	map<string, int>::iterator it = attributeLocList.find(attribute);
	 
	// Found it? Great -return the bound location! Didn't find it? Alert user and halt.
	if ( it != attributeLocList.end() )
	{
		return attributeLocList[attribute];
	}
	else
	{
		cerr << "Could not find attribute in shader program: " << attribute << endl;
	exit(-1);
	}
}
 
 
// Method to returns the bound location of a named uniform
GLuint ShaderProgram::uniform(const string uniform)
{
	// Note: You could do this method with the single line:
	//
	// 		return uniformLocList[uniform];
	//
	// But we're not doing that. Explanation in the attribute() method above.
	 
	// Create an iterator to look through our uniform map and try to find the named uniform
	static map<string, int>::iterator it = uniformLocList.find(uniform);
	 
	// Found it? Great - pass it back! Didn't find it? Alert user and halt.
	if ( it != uniformLocList.end() )
	{
		return uniformLocList[uniform];
	}
	else
	{
		cerr << "Could not find uniform in shader program: " << uniform << endl;
		exit(-1);
	}
}
 
 
// Method to add an attrbute to the shader and return the bound location
int ShaderProgram::addAttribute(const string attributeName)
{
	attributeLocList[attributeName] = glGetAttribLocation( ShaderProgram::programId, attributeName.c_str() );
	 
	// Check to ensure that the shader contains an attribute with this name
	if (attributeLocList[attributeName] == -1)
	{
		cerr << "Could not add attribute: " << attributeName << " - location returned -1!" << endl;
		exit(-1);
	}
	else
	{
		cout << "Attribute " << attributeName << " bound to location: " << attributeLocList[attributeName] << endl;
	}
	 
	return attributeLocList[attributeName];
}
 
 
// Method to add a uniform to the shader and return the bound location
int ShaderProgram::addUniform(const string uniformName)
{
	uniformLocList[uniformName] = glGetUniformLocation( ShaderProgram::programId, uniformName.c_str() );
	 
	// Check to ensure that the shader contains a uniform with this name
	if (uniformLocList[uniformName] == -1)
	{
		cerr << "Could not add uniform: " << uniformName << " - location returned -1!" << endl;
		exit(-1);
	}
	else
	{
		cout << "Uniform " << uniformName << " bound to location: " << uniformLocList[uniformName] << endl;
	}
	 
	return uniformLocList[uniformName];
}

