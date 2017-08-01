#ifndef __SHADER_H__
#define __SHADER_H__

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int Program;

	Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath);

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

	void setUniform3f(const std::string &name, glm::vec3 value);
	void setUniformMatrix4fv(const std::string &name, glm::mat4 transform);

	void setUniform3fv(const std::string &name, int nCount, const GLfloat *value);

	void setUniformBlockBindingIndex(const std::string &name, int bindingIndex);
};

#endif