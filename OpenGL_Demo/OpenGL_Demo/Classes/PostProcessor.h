#ifndef __POST_PROCESSOR_H__
#define __POST_PROCESSOR_H__

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "Shader.h"

class PostProcessor
{
public:
	PostProcessor(Shader shader, GLuint width, GLuint height);

	void beginRender();

	void endRender();

	void render(GLfloat dt);

public:
	Shader _postProcessingShader;
	Texture2D _texture;
	GLuint _width;
	GLuint _height;

	GLboolean _confuse;
	GLboolean _chaos;
	GLboolean _shake;

private:
	GLuint MSFBO, FBO;
	GLuint RBO;
	GLuint VAO;

	void initRenderData();
};

#endif