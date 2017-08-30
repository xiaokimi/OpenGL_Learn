#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

class GameLevel
{
public:
	GameLevel();

	void load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);

	void draw(SpriteRenderer &renderer);

	GLboolean isCompleted();

public:
	std::vector<GameObject> _bricks;

private:
	void init(std::vector<std::vector<GLuint> > tileData, GLuint levelWidth, GLuint levelHeight);
};

#endif