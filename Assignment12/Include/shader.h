#pragma once

#include <string>

#include <GL/glew.h>

//http://r3dux.org/2013/08/a-simple-c-opengl-shader-loader/

class Shader
{
private:

	GLuint id; 				// The unique ID / handle for the shader
	std::string typeString; 		// String representation of the shader type (i.e. "Vertex" or such)
	std::string source; 			// The shader source code (i.e. the GLSL code itself)

	bool compiled;
	bool loaded;

public:

	inline GLuint getId() { return id; }
	inline std::string getSource() { return source; }
	inline bool isCompiled() { return compiled; }
	inline bool hasSource() { return loaded; }

	Shader(const GLenum type);
	~Shader();

	void loadFromString(const std::string& sourceString);
	bool loadFromFile(const std::string& filename);

	bool compile();
}; //Shader