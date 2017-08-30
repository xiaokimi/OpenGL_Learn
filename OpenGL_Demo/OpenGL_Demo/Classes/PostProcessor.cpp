#include "PostProcessor.h"

#include <iostream>

PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height)
: _postProcessingShader(shader)
, _width(width)
, _height(height)
, _confuse(false)
, _chaos(false)
, _shake(false)
{
	glGenFramebuffers(1, &this->MSFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);

	glGenRenderbuffers(1, &this->RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

	glGenFramebuffers(1, &this->FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

	this->_texture.Generate(width, height, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_texture._textureID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->initRenderData();
	this->_postProcessingShader.use();
	this->_postProcessingShader.setInt("scene", 0);
	GLfloat offset = 1.0f / 300;
	GLfloat offsets[9][2] = {
		{-offset, offset},
		{0.0f, offset},
		{offset, offset},
		{-offset, 0.0f},
		{0.0f, 0.0f},
		{offset, 0.0f},
		{-offset, -offset},
		{0.0f, -offset},
		{offset, -offset}
	};
	glUniform2fv(glGetUniformLocation(this->_postProcessingShader.Program, "offsets"), 9, (GLfloat*)offsets);
	
	GLint edgeKernel[9] = {
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(this->_postProcessingShader.Program, "edgeKernel"), 9, edgeKernel);

	GLfloat blurKernel[9] = {
		1.0f / 16, 2.0f / 16, 1.0f / 16,
		2.0f / 16, 4.0f / 16, 2.0f / 16,
		1.0f / 16, 2.0f / 16, 1.0f / 16
	};
	glUniform1fv(glGetUniformLocation(this->_postProcessingShader.Program, "blurKernel"), 9, blurKernel);
}

void PostProcessor::beginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::endRender()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->_width, this->_height, 0, 0, this->_width, this->_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::render(GLfloat dt)
{
	Shader &shader = this->_postProcessingShader;
	shader.use();
	shader.setFloat("time", dt);
	shader.setBool("confuse", this->_confuse);
	shader.setBool("chaos", this->_chaos);
	shader.setBool("shake", this->_shake);

	glActiveTexture(GL_TEXTURE0);
	this->_texture.Bind();

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f
	};

	GLuint VBO;

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}