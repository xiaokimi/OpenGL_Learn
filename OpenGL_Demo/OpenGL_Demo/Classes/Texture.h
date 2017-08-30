#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GL/glew.h>

class Texture2D
{
public:
	GLuint _textureID;

	GLuint _internal_Format;
	GLuint _image_Format;

public:
	Texture2D();

	void Generate(GLuint width, GLuint height, GLubyte *data);

	void Bind() const;

private:
	GLuint _width, _height;

	GLuint _Wrap_S;
	GLuint _Wrap_T;
	GLuint _Filter_Min;
	GLuint _Filter_Max;
};

#endif