#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "stb_image.h"

std::map<std::string, Texture2D> ResourceManager::_textures;
std::map<std::string, Shader> ResourceManager::_shaders;

Shader ResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	_shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return _shaders[name];
}

Shader ResourceManager::getShader(std::string name)
{
	auto iter = _shaders.find(name);
	if (iter != _shaders.end())
	{
		return _shaders[name];
	}
	else
	{
		std::cout << "ERROR::FIND_SHADER_ERROE: " << name << std::endl;
		return Shader();
	}
}

Texture2D ResourceManager::loadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	_textures[name] = loadTextureFromFile(file, alpha);
	return _textures[name];
}

Texture2D ResourceManager::getTexture(std::string name)
{
	auto iter = _textures.find(name);
	if (iter != _textures.end())
	{
		return _textures[name];
	}
	else
	{
		std::cout << "ERROR::FIND_TEXTURE_ERROE: " << name << std::endl;
		return Texture2D();
	}
}

void ResourceManager::clear()
{
	for (auto iter : _shaders)
	{
		glDeleteProgram(iter.second.Program);
	}

	for (auto iter : _textures)
	{
		glDeleteTextures(1, &iter.second._textureID);
	}
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile /* = nullptr */)
{
	return Shader(vShaderFile, fShaderFile);
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	Texture2D texture;
	if (alpha)
	{
		texture._internal_Format = GL_RGBA;
		texture._image_Format = GL_RGBA;
	}

	int nWidth, nHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(file, &nWidth, &nHeight, &nrChannels, 0);
	if (data)
	{
		texture.Generate(nWidth, nHeight, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}