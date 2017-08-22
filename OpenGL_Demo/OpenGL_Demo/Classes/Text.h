#ifndef __TEXT_H__
#define __TEXT_H__

#include <windows.h>

#include <iostream>
#include <string>
#include <map>

#define GLEW_STATIC
#include <gl/glew.h>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct Character
{
	glm::ivec2 Position;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class Text
{
public:
	static Text *getInstance();

	void init(const char* fontFile = "Resource/Fonts/STFANGSO.TTF", int fontSize = 24);

	void RenderText(Shader &shader, wchar_t* textStr, glm::vec2 position, glm::vec2 scale, glm::vec3 color);
protected:
	Text();
	~Text();

	Character* getCharacter(wchar_t ch);
private:
	GLuint VAO, VBO, TextureID;

	FT_Library _library;
	FT_Face _face;

	int _xStart;
	int _yStart;
	int _fontSize;
	std::map<wchar_t, Character> Characters;
};

#endif