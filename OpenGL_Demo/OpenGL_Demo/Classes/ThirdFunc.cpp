#include <iostream>

#include "ThirdFunc.h"
#include "stb_image.h"

using namespace std;

GLuint loadTexTure(const char *filename, GLint internalformat, GLint format, bool fliptag /* = false */)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nWidth, nHeight, nrChannels;
	stbi_set_flip_vertically_on_load(fliptag);
	unsigned char *data = stbi_load(filename, &nWidth, &nHeight, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, nWidth, nHeight, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint loadCubemap(vector<const GLchar*> faces, GLint internalformat, GLint format, bool fliptag /* = false */)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (GLuint i = 0; i < faces.size(); i++)
	{
		int nWidth, nHeight, nrChannels;
		stbi_set_flip_vertically_on_load(fliptag);
		unsigned char *data = stbi_load(faces[i], &nWidth, &nHeight, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalformat, nWidth, nHeight, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

GLuint getBufferDataFromFile(const char *filename)
{
	GLuint buffer;
	FILE *f;
	size_t filesize;

	fopen_s(&f, filename, "rb");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		filesize = ftell(f);

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_COPY_WRITE_BUFFER, buffer);
		glBufferData(GL_COPY_WRITE_BUFFER, (GLsizei)filesize, NULL, GL_STATIC_DRAW);

		void *data = glMapBuffer(GL_COPY_WRITE_BUFFER, GL_WRITE_ONLY);
		fread(data, 1, filesize, f);

		glUnmapBuffer(GL_COPY_WRITE_BUFFER);

		fclose(f);

		return buffer;
	}
	else
	{
		std::cout << "READ FILE FAILED : " << filename << endl;
	}
	
	return 0;
}