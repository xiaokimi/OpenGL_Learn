#include "GameObject.h"

GameObject::GameObject()
: _position(0.0f, 0.0f)
, _size(1.0f, 1.0f)
, _velocity(0.0f, 0.0f)
, _color(1.0f, 1.0f, 1.0f)
, _rotation(0.0f)
, _sprite()
, _isSolid(false)
, _destroyed(false)
{

}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color /* = glm::vec3(1.0f) */, glm::vec2 velocity /* = glm::vec2(0.0f) */)
: _position(pos)
, _size(size)
, _velocity(velocity)
, _color(color)
, _rotation(0.0f)
, _sprite(sprite)
, _isSolid(false)
, _destroyed(false)
{

}

void GameObject::draw(SpriteRenderer &renderer)
{
	renderer.drawSprite(this->_sprite, this->_position, this->_size, glm::radians(this->_rotation), this->_color);
}