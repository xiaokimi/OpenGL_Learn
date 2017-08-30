#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <map>
#include <string>

#include <GL/glew.h>

#include "Texture.h"
#include "Shader.h"

class ResourceManager
{
public:
	static Shader loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);

	static Shader getShader(std::string name);

	static Texture2D loadTexture(const GLchar *file, GLboolean alpha, std::string name);

	static Texture2D getTexture(std::string name);

	static void clear();

private:
	ResourceManager() {}

	static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);

	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);

private:
	static std::map<std::string, Shader> _shaders;
	static std::map<std::string, Texture2D> _textures;
};

#endif