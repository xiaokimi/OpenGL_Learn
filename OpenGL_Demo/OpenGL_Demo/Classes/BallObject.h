#ifndef __BALL_OBJECT_H__
#define __BALL_OBJECT_H__

#include <Gl/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "PowerUp.h"

class BallObject : public GameObject
{
public:
	BallObject();

	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 move(GLfloat dt, GLuint windowWidth);

	void reset(glm::vec2 position, glm::vec2 velocity);

public:
	GLfloat _radius;
	GLboolean _stuck;

	GLboolean _sticky;
	GLboolean _passThrough;
};

#endif