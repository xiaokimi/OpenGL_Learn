#include "Text.h"
#include "ResourceManager.h"

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
: _xStart(0)
, _yStart(0)
, _fontSize(24)
{

}

Text::~Text()
{
	if (_text)
	{
		delete _text;
		_text = nullptr;

		FT_Done_Face(_face);
		FT_Done_FreeType(_library);

		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}

	release();
}

void Text::init(const char* fontFile /* = "Resource/Fonts/STFANGSO.TTF" */, int fontSize /* = 24 */)
{
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	this->_textShader = ResourceManager::loadShader("Shader/2DGame/fonts.vs", "Shader/2DGame/fonts.frag", nullptr, "text");
	this->_textShader.use();
	this->_textShader.setUniformMatrix4fv("projection", projection);

	_fontSize = fontSize;

	if (FT_Init_FreeType(&_library))
	{
		cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;
	}

	if (FT_New_Face(_library, fontFile, 0, &_face))
	{
		cout << "ERROR::FREETYPE: Failed to load font" << endl;
	}

	FT_Select_Charmap(_face, FT_ENCODING_UNICODE);

	FT_Set_Pixel_Sizes(_face, 0, fontSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// texture init
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		1024,
		1024,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// VAO init
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 6 * 4 * 1024, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::RenderText(char* str, glm::vec2 position, glm::vec2 scale, glm::vec3 color)
{
	Shader &shader = this->_textShader;
	shader.use();
	shader.setUniform3f("textColor", color);

	glActiveTexture(GL_TEXTURE0);
	shader.setInt("text", 0);

	glBindVertexArray(VAO);

	GLfloat x = position.x;
	GLfloat y = position.y;

	glBindTexture(GL_TEXTURE_2D, TextureID);

	wchar_t *textStr = charToWchar(str);
	size_t nLen = wcslen(textStr);
	for (int i = 0; i < nLen; i++)
	{
		Character *ch = getCharacter(textStr[i]);

		glm::ivec2 pos = ch->Position;

		GLfloat xpos = x + ch->Bearing.x * scale.x;
		GLfloat ypos = y - (ch->Size.y - ch->Bearing.y) * scale.y;

		GLfloat w = ch->Size.x * scale.x;
		GLfloat h = ch->Size.y * scale.y;
		/**
		** freetype 存储在纹理中的数据, uv 坐标的顶点在左上角
		** 	y				p0---p3
		**	|				| \  |
		**	|				|  \ |
		**	|				|   \|
		**	|________x		p1---p2
		*/ 
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, pos.x / 1024.0f, pos.y / 1024.0f },
			{ xpos, ypos, pos.x / 1024.0f, (pos.y + ch->Size.y) / 1024.0f },
			{ xpos + w, ypos, (pos.x + ch->Size.x) / 1024.0f, (pos.y + ch->Size.y) / 1024.0f },

			{ xpos + w, ypos, (pos.x + ch->Size.x) / 1024.0f, (pos.y + ch->Size.y) / 1024.0f },
			{ xpos + w, ypos + h, (pos.x + ch->Size.x) / 1024.0f, pos.y / 1024.0f },
			{ xpos, ypos + h, pos.x / 1024.0f, pos.y / 1024.0f }
		};

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) * i, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		x += (ch->Advance >> 6) * scale.x;
	}

	glDrawArrays(GL_TRIANGLES, 0, 6 * nLen);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Character* Text::getCharacter(wchar_t ch)
{
	Character character = Characters[ch];
	if (character.Position == glm::ivec2(0) && character.Size == glm::ivec2(0))
	{
		if (_xStart + _fontSize > 1024)
		{
			_xStart = 0;
			_yStart += _fontSize;
		}

		if (FT_Load_Char(_face, ch, FT_LOAD_RENDER))
		{
			character.Advance = 0;
			Characters[ch] = character;

			_xStart += _fontSize / 2;
		}
		else
		{
			int w = _face->glyph->bitmap.width;
			int h = _face->glyph->bitmap.rows;

			int xoffset = _face->glyph->bitmap_left;
			int yoffset = _face->glyph->bitmap_top;

			int advance = _face->glyph->advance.x;

			character.Position = glm::ivec2(_xStart, _yStart);
			character.Size = glm::ivec2(w, h);
			character.Bearing = glm::ivec2(xoffset, yoffset);
			character.Advance = advance;		

			Characters[ch] = character;
			glBindTexture(GL_TEXTURE_2D, TextureID);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexSubImage2D(
				GL_TEXTURE_2D,
				0,
				_xStart,
				_yStart,
				w,
				h,
				GL_RED,
				GL_UNSIGNED_BYTE,
				_face->glyph->bitmap.buffer);

			_xStart += w + 1;
		}
	}

	return &Characters[ch];
}

void Text::release()
{
	if (m_char)
	{
		delete m_char;
		m_char = nullptr;
	}

	if (m_wchar)
	{
		delete m_wchar;
		m_wchar = nullptr;
	}
}

char* Text::wcharToChar(wchar_t* wc)
{
	release();

	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

wchar_t* Text::charToWchar(char* c)
{
	release();

	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}