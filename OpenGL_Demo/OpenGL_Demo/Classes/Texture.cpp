#include <iostream>

#include "Texture.h"

Texture2D::Texture2D()
: _width(0)
, _height(0)
, _internal_Format(GL_RGB)
, _image_Format(GL_RGB)
, _Wrap_S(GL_REPEAT)
, _Wrap_T(GL_REPEAT)
, _Filter_Min(GL_LINEAR)
, _Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &this->_textureID);
}

void Texture2D::Generate(GLuint width, GLuint height, GLubyte *data)
{
	this->_width = width;
	this->_height = height;

	glBindTexture(GL_TEXTURE_2D, this->_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->_internal_Format, width, height, 0, this->_image_Format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->_Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->_Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->_Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->_Filter_Max);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->_textureID);
}
