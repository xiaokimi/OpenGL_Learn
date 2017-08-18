#ifndef __TEXT_H__
#define __TEXT_H__

#include <iostream>
#include <string>
#include <map>

#define GLEW_STATIC
#include <gl/glew.h>

#include <glm/glm.hpp>

#include "Shader.h"

struct Character
{
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class Text
{
public:
	static Text *getInstance();

	void init();

	void RenderText(Shader &shader, std::string text, glm::vec2 position, glm::vec2 scale, glm::vec3 color);
protected:
	Text();
	~Text();

private:
	GLuint VAO, VBO;
	std::map<GLchar, Character> Characters;
};

#endif