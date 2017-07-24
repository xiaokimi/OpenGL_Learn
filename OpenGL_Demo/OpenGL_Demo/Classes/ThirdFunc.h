#ifndef __THIRD_FUNC_H__
#define __THIRD_FUNC_H__

#include <vector>
#include <gl/glew.h>

GLuint loadTexTure(const char *filename, GLint internalformat, GLint format, bool fliptag = false);

GLuint loadCubemap(std::vector<const GLchar*> faces, GLint internalformat, GLint format, bool fliptag = false);

#endif