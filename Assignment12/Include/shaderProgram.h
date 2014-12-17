#pragma once

#include <GL/glew.h>

#include "shader.h"

//http://r3dux.org/2013/08/a-simple-c-opengl-shader-loader/

class ShaderProgram
{
private:

	GLuint programId; 	// The unique ID / handle for the shader program

	GLuint shaderCount; // How many shaders are attached to the shader program

	bool linked;

public:

	inline GLuint getId() { return programId; }
	inline bool isLinked() { return linked; }
	inline int getShaderCount() { return shaderCount; }

	ShaderProgram();
	virtual ~ShaderProgram();

	void attachShader(Shader& shader);

	bool link();
	bool validate();

	bool getBoolParameter(GLenum pname);
	GLint getIntParameter(GLenum pname);

	inline bool getDeleteStatus()
	{
		return getBoolParameter(GL_DELETE_STATUS);
	} //getDeleteStatus

	inline bool isValidated()
	{
		return getBoolParameter(GL_VALIDATE_STATUS);
	} //getValidateStatus

	inline GLint getInfoLogLength()
	{
		return getBoolParameter(GL_INFO_LOG_LENGTH);
	} //getInfoLogLength

	inline GLint getActiveAttributeCount()
	{
		return getBoolParameter(GL_ACTIVE_ATTRIBUTES);
	} //getActiveAttributes

	inline GLint getActiveAttributeMaxLength()
	{
		return getBoolParameter(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
	} //getActiveAttributeMaxLength

	inline GLint getActiveUniformCount()
	{
		return getBoolParameter(GL_ACTIVE_UNIFORMS);
	} //getActiveUniformCount

	inline GLint getActiveUniformMaxLength()
	{
		return getBoolParameter(GL_ACTIVE_UNIFORM_MAX_LENGTH);
	} //getActiveUniformMaxLength

	// Method to enable the shader program
	inline void bind()
	{
		glUseProgram(programId);
	} //use

	// Method to disable the shader program
	inline void unbind()
	{
		glUseProgram(0);
	} //disable

	// Returns the bound location of a named attribute
	inline GLuint attributeLocation(const std::string& attribute)
	{
		return glGetAttribLocation(programId, attribute.c_str());
	} //attribute

	// Method to returns the bound location of a named uniform
	inline GLuint uniformLocation(const std::string& uniform)
	{
		return glGetUniformLocation(programId, uniform.c_str());
	} //uniform

	inline void enable(GLuint attribute)
	{
		glEnableVertexAttribArray(attribute);
	} //enable

	inline void setUniform(GLuint location, GLfloat v0)
	{
		glUniform1f(location, v0);
	} //setUniform

	inline void setUniform(GLuint location, GLfloat v0, GLfloat v1)
	{
		glUniform2f(location, v0, v1);
	} //setUniform

	inline void setUniform(GLuint location, GLfloat v0, GLfloat v1, GLfloat v2)
	{
		glUniform3f(location, v0, v1, v2);
	} //setUniform

	inline void setUniform(GLuint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
	{
		glUniform4f(location, v0, v1, v2, v3);
	} //setUniform


	inline void setUniform(GLuint location, GLint v0)
	{
		glUniform1i(location, v0);
	} //setUniform

	inline void setUniform(GLuint location, GLint v0, GLint v1)
	{
		glUniform2i(location, v0, v1);
	} //setUniform

	inline void setUniform(GLuint location, GLint v0, GLint v1, GLint v2)
	{
		glUniform3i(location, v0, v1, v2);
	} //setUniform

	inline void setUniform(GLuint location, GLint v0, GLint v1, GLint v2, GLint v3)
	{
		glUniform4i(location, v0, v1, v2, v3);
	} //setUniform

	inline void setUniform1(GLint location, GLsizei count, const GLfloat* value)
	{
		glUniform1fv(location, count, value);
	} //setUniform1
	inline void setUniform2(GLint location, GLsizei count, const GLfloat* value)
	{
		glUniform2fv(location, count, value);
	} //setUniform2
	inline void setUniform3(GLint location, GLsizei count, const GLfloat* value)
	{
		glUniform3fv(location, count, value);
	} //setUniform3
	inline void setUniform4(GLint location, GLsizei count, const GLfloat* value)
	{
		glUniform4fv(location, count, value);
	} //setUniform4

	inline void setUniform1(GLint location, GLsizei count, const GLint* value)
	{
		glUniform1iv(location, count, value);
	} //setUniform1
	inline void setUniform2(GLint location, GLsizei count, const GLint* value)
	{
		glUniform2iv(location, count, value);
	} //setUniform2
	inline void setUniform3(GLint location, GLsizei count, const GLint* value)
	{
		glUniform3iv(location, count, value);
	} //setUniform3
	inline void setUniform4(GLint location, GLsizei count, const GLint* value)
	{
		glUniform4iv(location, count, value);
	} //setUniform4

	inline void setUniform2(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix2fv(location, count, transpose, value);
	} //setUniform2
	inline void setUniform3(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix3fv(location, count, transpose, value);
	} //setUniform3
	inline void setUniform4(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix4fv(location, count, transpose, value);
	} //setUniform4
	inline void setUniform2x3(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix2x3fv(location, count, transpose, value);
	} //setUniform2x3
	inline void setUniform3x2(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix3x2fv(location, count, transpose, value);
	} //setUniform3x2
	inline void setUniform2x4(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix2x4fv(location, count, transpose, value);
	} //setUniform2x4
	inline void setUniform4x2(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix4x2fv(location, count, transpose, value);
	} //setUniform4x2
	inline void setUniform3x4(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix3x4fv(location, count, transpose, value);
	} //setUniform3x4
	inline void setUniform4x3(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
	{
		glUniformMatrix4x3fv(location, count, transpose, value);
	} //setUniform4x3

	inline void setAttribute(GLuint location, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
	{
		glVertexAttribPointer(location, size, type, normalized, stride, pointer);
	} //setAttribute
}; //ShaderProgram