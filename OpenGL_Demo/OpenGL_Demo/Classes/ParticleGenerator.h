#ifndef __PARTICLE_GENERATOR_H__
#define __PARTICLE_GENERATOR_H__

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

struct Particle
{
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);

	void update(GLfloat dt, GameObject &obj, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

	void draw();

private:
	void init();

	GLuint firstUnusedParticle();

	void respawnParticle(Particle &particle, GameObject &obj, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

private:
	std::vector<Particle> _particles;

	GLuint _amount;

	Shader _shader;
	Texture2D _texture;

	GLuint VAO;
};

#endif