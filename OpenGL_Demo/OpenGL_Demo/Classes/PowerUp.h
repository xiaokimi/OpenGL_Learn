#ifndef __POWER_UP_H__
#define __POWER_UP_H__

#include <string>

#include <Gl/glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"

const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
	PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
		: GameObject(position, POWERUP_SIZE, texture, color, VELOCITY)
		, _type(type)
		, _duration(duration)
		, _activated()
	{

	}

public:
	std::string _type;
	GLfloat _duration;
	GLboolean _activated;
};

#endif