#include "BallObject.h"

BallObject::BallObject()
: GameObject()
, _radius(12.5f)
, _stuck(GL_TRUE)
, _sticky(GL_FALSE)
, _passThrough(GL_FALSE)
{

}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
: GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity)
, _radius(radius)
, _stuck(GL_TRUE)
, _sticky(GL_FALSE)
, _passThrough(GL_FALSE)
{

}

glm::vec2 BallObject::move(GLfloat dt, GLuint windowWidth)
{
	if (!this->_stuck)
	{
		this->_position += this->_velocity * dt;

		if (this->_position.x <= 0.0f)
		{
			this->_velocity.x = -this->_velocity.x;
			this->_position.x = 0.0f;
		}
		else if (this->_position.x + this->_size.x >= windowWidth)
		{
			this->_velocity.x = -this->_velocity.x;
			this->_position.x = windowWidth - this->_size.x;
		}

		if (this->_position.y <= 0.0f)
		{
			this->_velocity.y = -this->_velocity.y;
			this->_position.y = 0.0f;
		}
	}
	return this->_position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity)
{
	this->_position = position;
	this->_velocity = velocity;
	this->_stuck = GL_TRUE;
	this->_sticky = GL_FALSE;
	this->_passThrough = GL_FALSE;
}