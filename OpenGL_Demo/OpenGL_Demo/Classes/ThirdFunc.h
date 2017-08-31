#ifndef __THIRD_FUNC_H__
#define __THIRD_FUNC_H__

#include <vector>
#include <gl/glew.h>

#include <glm/glm.hpp>

GLuint loadTexTure(const char *filename, GLint internalformat, GLint format, bool fliptag = false);

GLuint loadCubemap(std::vector<const GLchar*> faces, GLint internalformat, GLint format, bool fliptag = false);

GLuint getBufferDataFromFile(const char *filename);

void calcTangent(glm::vec3 *positions, glm::vec2 *texCoords, glm::vec3 &tangent, glm::vec3 &bitangent);

void loadFileMemory(const char* filename, void **buff, int *length);

#endif