#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
: _shader(shader)
, _texture(texture)
, _amount(amount)
{
	this->init();
}

void ParticleGenerator::update(GLfloat dt, GameObject &obj, GLuint newParticles, glm::vec2 offset /* = glm::vec2(0.0f, 0.0f) */)
{
	for (GLuint i = 0; i < newParticles; i++)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->_particles[unusedParticle], obj, offset);
	}

	for (GLuint i = 0; i < this->_amount; i++)
	{
		Particle &p = this->_particles[i];
		p.Life -= dt;
		if (p.Life > 0.0f)
		{
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5;
		}
	}
}

void ParticleGenerator::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->_shader.use();

	for (Particle particle : this->_particles)
	{
		if (particle.Life > 0.0f)
		{
			this->_shader.setUniform2f("offset", particle.Position);
			this->_shader.setUniform4f("color", particle.Color);
			
			this->_texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	GLfloat particleQuad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	GLuint VBO;

	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	for (GLuint i = 0; i < this->_amount; i++)
	{
		this->_particles.push_back(Particle());
	}
}

GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	for (GLuint i = lastUsedParticle; i < this->_amount; i++)
	{
		if (this->_particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	for (GLuint i = 0; i < lastUsedParticle; i++)
	{
		if (this->_particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObject &obj, glm::vec2 offset /* = glm::vec2(0.0f, 0.0f) */)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);

	particle.Position = obj._position + random + offset;
	particle.Color = glm::vec4(glm::vec3(rColor), 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = obj._velocity * 0.1f;
}