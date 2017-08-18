#include "Text.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

Text *_text = nullptr;

Text *Text::getInstance()
{
	if (!_text)
	{
		_text = new Text();
	}

	return _text;
}

Text::Text()
{

}

Text::~Text()
{
	if (_text)
	{
		delete _text;
		_text = nullptr;
	}
}

void Text::init()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, "Resource/Fonts/arial.ttf", 0, &face))
	{
		cout << "ERROR::FREETYPE: Failed to load font" << endl;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			cout << "ERROR::FREETYPE: Failed to load Glyph" << endl;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(pair<GLchar, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::RenderText(Shader &shader, std::string text, glm::vec2 position, glm::vec2 scale, glm::vec3 color)
{
	shader.use();
	shader.setUniform3f("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	shader.setInt("text", 0);

	glBindVertexArray(VAO);

	GLfloat x = position.x;
	GLfloat y = position.y;

	string::const_iterator c = text.begin();
	for (; c != text.end(); ++c)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale.x;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale.y;

		GLfloat w = ch.Size.x * scale.x;
		GLfloat h = ch.Size.y * scale.y;

		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale.x;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}