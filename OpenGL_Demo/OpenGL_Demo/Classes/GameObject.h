#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <gl/glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	GameObject();

	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));

	virtual void draw(SpriteRenderer &renderer);

public:
	glm::vec2 _position;
	glm::vec2 _size;
	glm::vec2 _velocity;
	glm::vec3 _color;

	GLfloat _rotation;
	GLboolean _isSolid;
	GLboolean _destroyed;

	Texture2D _sprite;
};

#endif