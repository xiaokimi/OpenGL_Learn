#include <iostream>

#include "ThirdFunc.h"
#include "stb_image.h"

using namespace std;

GLuint loadTexTure(const char *filename, GLint internalformat, GLint format, bool fliptag /* = false */)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

void calcTangent(glm::vec3 *positions, glm::vec2 *texCoords, glm::vec3 &tangent, glm::vec3 &bitangent)
{
	// Shortcuts for vertices
	glm::vec3 & v0 = positions[0];
	glm::vec3 & v1 = positions[1];
	glm::vec3 & v2 = positions[2];

	// Shortcuts for UVs
	glm::vec2 & uv0 = texCoords[0];
	glm::vec2 & uv1 = texCoords[1];
	glm::vec2 & uv2 = texCoords[2];

	// Edges of the triangle : postion delta
	glm::vec3 deltaPos1 = v1 - v0;
	glm::vec3 deltaPos2 = v2 - v0;

	// UV delta
	glm::vec2 deltaUV1 = uv1 - uv0;
	glm::vec2 deltaUV2 = uv2 - uv0;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	tangent = glm::normalize((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r);
	bitangent = glm::normalize((deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r);
}