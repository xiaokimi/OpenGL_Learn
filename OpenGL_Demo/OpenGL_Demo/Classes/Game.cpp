#include <algorithm>

#include "Game.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "Text.h"

#include <irrklang/irrKlang.h>
using namespace irrklang;

ISoundEngine *SoundEngine = createIrrKlangDevice();

SpriteRenderer *spriteRender;
GameObject *player;
BallObject *Ball;
ParticleGenerator *particles;
PostProcessor *effects;
GLfloat ShakeTime = 0.0f;

void activePowerUp(PowerUp &powerUp);

Game::Game(GLuint width, GLuint height)
: _state(GAME_MENU)
, _keys()
, _width(width)
, _height(height)
, _lives(5)
{

}

Game::~Game()
{
	if (spriteRender)
	{
		delete spriteRender;
		spriteRender = nullptr;
	}

	if (player)
	{
		delete player;
		player = nullptr;
	}

	if (Ball)
	{
		delete Ball;
		Ball = nullptr;
	}

	if (particles)
	{
		delete particles;
		particles = nullptr;
	}

	if (effects)
	{
		delete effects;
		effects = nullptr;
	}

	if (SoundEngine)
	{
		SoundEngine->drop();
	}
}

void Game::init()
{
	srand(glfwGetTime());

	Shader shader = ResourceManager::loadShader("Shader/2DGame/spriteShader.vs", "Shader/2DGame/spriteShader.frag", nullptr, "sprite");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->_width), static_cast<GLfloat>(this->_height), 0.0f, -1.0f, 1.0f);
	
	shader.use();
	shader.setInt("image", 0);
	shader.setUniformMatrix4fv("projection", projection);

	spriteRender = new SpriteRenderer(shader);

	Shader particleShader = ResourceManager::loadShader("Shader/2DGame/particle.vs", "Shader/2DGame/particle.frag", nullptr, "particle");
	particleShader.use();
	particleShader.setInt("sprite", 0);
	particleShader.setUniformMatrix4fv("projection", projection);

	ResourceManager::loadShader("Shader/2DGame/postProcessing.vs", "Shader/2DGame/postProcessing.frag", nullptr, "postProcessing");

	// load texture
	ResourceManager::loadTexture("Resource/Image/background.jpg", GL_FALSE, "background");
	ResourceManager::loadTexture("Resource/Image/Pea.png", GL_TRUE, "face");
	ResourceManager::loadTexture("Resource/Image/block.png", GL_FALSE, "block");
	ResourceManager::loadTexture("Resource/Image/block_solid.png", GL_FALSE, "blockSolid");
	ResourceManager::loadTexture("Resource/Image/paddle.png", GL_TRUE, "paddle");
	ResourceManager::loadTexture("Resource/Image/particle.png", GL_TRUE, "particle");

	ResourceManager::loadTexture("Resource/Image/powerup_speed.png", GL_TRUE, "powerupSpeed");
	ResourceManager::loadTexture("Resource/Image/powerup_sticky.png", GL_TRUE, "powerupSticky");
	ResourceManager::loadTexture("Resource/Image/powerup_increase.png", GL_TRUE, "powerupIncrease");
	ResourceManager::loadTexture("Resource/Image/powerup_confuse.png", GL_TRUE, "powerupConfuse");
	ResourceManager::loadTexture("Resource/Image/powerup_chaos.png", GL_TRUE, "powerupChaos");
	ResourceManager::loadTexture("Resource/Image/powerup_passthrough.png", GL_TRUE, "powerupPassthrough");

	GameLevel one;
	one.load("Level/one.lvl", this->_width, this->_height * 0.5f);
	GameLevel two;
	two.load("Level/two.lvl", this->_width, this->_height * 0.5f);
	GameLevel three;
	three.load("Level/three.lvl", this->_width, this->_height * 0.5f);
	GameLevel four;
	four.load("Level/four.lvl", this->_width, this->_height * 0.5f);

	this->_levels.push_back(one);
	this->_levels.push_back(two);
	this->_levels.push_back(three);
	this->_levels.push_back(four);

	this->_level = 0;

	// player init
	glm::vec2 playerPos = glm::vec2(this->_width / 2 - PLAYER_SIZE.x / 2, this->_height - PLAYER_SIZE.y);
	player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));

	// ball init
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face"));

	// particle
	particles = new ParticleGenerator(
		ResourceManager::getShader("particle"),
		ResourceManager::getTexture("particle"),
		500);

	effects = new PostProcessor(ResourceManager::getShader("postProcessing"), this->_width, this->_height);

	SoundEngine->loadPlugins("E:/GitHub/OpenGL_Learn/OpenGL_Lib/Libs");
	SoundEngine->play2D("Resource/Music/breakout.mp3", true);

	Text::getInstance()->init("Resource/Fonts/STFANGSO.TTF", 50);
}

void Game::update(GLfloat dt)
{
	Ball->move(dt, this->_width);

	this->doCollision();

	particles->update(dt, *Ball, 2, glm::vec2(Ball->_radius / 2));

	this->updatePowerUps(dt);

	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0)
		{
			effects->_shake = false;
		}
	}

	if (Ball->_position.y >= this->_height)
	{
		--this->_lives;
		if (this->_lives == 0)
		{
			this->resetLevel();
			this->_state = GAME_MENU;
		}
		this->resetPlayer();
	}

	if (this->_state == GAME_ACTIVE && this->_levels[this->_level].isCompleted())
	{
		this->resetLevel();
		this->resetPlayer();
		effects->_chaos = GL_TRUE;
		this->_state = GAME_WIN;
	}
}

void Game::processInput(GLfloat dt)
{
	if (this->_state == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		if (this->_keys[GLFW_KEY_A])
		{
			if (player->_position.x >= 0)
			{
				player->_position.x -= velocity;
				
				if (Ball->_stuck)
				{
					Ball->_position.x -= velocity;
				}
			}
		}

		if (this->_keys[GLFW_KEY_D])
		{
			if (player->_position.x <= this->_width - player->_size.x)
			{
				player->_position.x += velocity;

				if (Ball->_stuck)
				{
					Ball->_position.x += velocity;
				}
			}
		}

		if (this->_keys[GLFW_KEY_SPACE])
		{
			Ball->_stuck = GL_FALSE;
		}
	}
	else if (this->_state == GAME_MENU)
	{
		if (this->_keys[GLFW_KEY_ENTER] && !this->_keyProcessed[GLFW_KEY_ENTER])
		{
			this->_state = GAME_ACTIVE;
			this->_keyProcessed[GLFW_KEY_ENTER] = true;
		}

		if (this->_keys[GLFW_KEY_W] && !this->_keyProcessed[GLFW_KEY_W])
		{
			this->_level = (this->_level + 1) % 4;
			this->_keyProcessed[GLFW_KEY_W] = true;
		}

		if (this->_keys[GLFW_KEY_S] && !this->_keyProcessed[GLFW_KEY_S])
		{
			this->_level = (this->_level + 3) % 4;
			this->_keyProcessed[GLFW_KEY_S] = true;
		}
	}
	else if (this->_state == GAME_WIN)
	{
		if (this->_keys[GLFW_KEY_ENTER])
		{
			this->_keyProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			effects->_chaos = GL_FALSE;
			this->_state = GAME_MENU;
		}
	}
}

void Game::render()
{
	if (this->_state == GAME_ACTIVE || this->_state == GAME_MENU || this->_state == GAME_WIN)
	{
		effects->beginRender();

			Texture2D background = ResourceManager::getTexture("background");
			spriteRender->drawSprite(background, glm::vec2(0.0f, 0.0f), glm::vec2(this->_width, this->_height), 0.0f);

			this->_levels[this->_level].draw(*spriteRender);

			player->draw(*spriteRender);
	
			for (PowerUp &powerUp : this->_powerUps)
			{
				if (!powerUp._destroyed)
				{
					powerUp.draw(*spriteRender);
				}
			}

			particles->draw();

			Ball->draw(*spriteRender);

		effects->endRender();

		effects->render(glfwGetTime());

		char str[50] = {};
		sprintf_s(str, 50, "ÉúÃüÖµ£º%d", this->_lives);
		Text::getInstance()->RenderText(str, glm::vec2(5.0f, this->_height - 25.0f), glm::vec2(0.5f), glm::vec3(1.0f));
	}

	if (this->_state == GAME_MENU)
	{
		Text::getInstance()->RenderText("Press ENTER to start", glm::vec2(300.0f, this->_height/2 - 25.0f), glm::vec2(0.5f), glm::vec3(1.0f));
		Text::getInstance()->RenderText("Press W or S to select level", glm::vec2(275.0f, this->_height / 2 - 50.0f), glm::vec2(0.5f), glm::vec3(1.0f));
	}

	if (this->_state == GAME_WIN)
	{
		Text::getInstance()->RenderText("You Win !!!", glm::vec2(300.0f, this->_height / 2 - 25.0f), glm::vec2(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void Game::keyCallback(int key, int scancode, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			_keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			_keys[key] = false;
			_keyProcessed[key] = false;
		}
			
	}
}

GLboolean Game::checkCollision(GameObject &one, GameObject &two)
{
	bool collisionX = one._position.x + one._size.x >= two._position.x &&
		two._position.x + two._size.x >= one._position.x;

	bool collisionY = one._position.y + one._size.y >= two._position.y &&
		two._position.y + two._size.y >= one._position.y;

	return collisionX && collisionY;
}

Collision Game::checkCollision(BallObject &ball, GameObject &obj)
{
	glm::vec2 center(ball._position + ball._radius);

	glm::vec2 aabb_half_extents(obj._size.x / 2, obj._size.y / 2);

	glm::vec2 aabb_center(obj._position + aabb_half_extents);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;

	if (glm::length(difference) < ball._radius)
	{
		return std::make_tuple(GL_TRUE, vectorDirection(difference), difference);
	}
	else
	{
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0.0f, 0.0f));
	}
}

void Game::doCollision()
{
	for (GameObject &box : this->_levels[this->_level]._bricks)
	{
		if (!box._destroyed)
		{
			Collision collision = checkCollision(*Ball, box);
			if (std::get<0>(collision))
			{
				if (!box._isSolid)
				{
					box._destroyed = GL_TRUE;
					this->spawnPowerUps(box);
					SoundEngine->play2D("Resource/Music/bleep.mp3", false);
				}
				else
				{
					ShakeTime = 0.05f;
					effects->_shake = true;
					SoundEngine->play2D("Resource/Music/solid.wav", false);
				}

				Direction dir = std::get<1>(collision);
				glm::vec2 diffVector = std::get<2>(collision);
				if (!(Ball->_passThrough && !box._isSolid))
				{
					if (dir == Left || dir == Right)
					{
						Ball->_velocity.x = -Ball->_velocity.x;

						GLfloat penetration = Ball->_radius - std::abs(diffVector.x);
						if (dir == Left)
							Ball->_position.x += penetration;
						else
							Ball->_position.x -= penetration;
					}
					else
					{
						Ball->_velocity.y = -Ball->_velocity.y;

						GLfloat penetration = Ball->_radius - std::abs(diffVector.y);
						if (dir == UP)
							Ball->_position.y -= penetration;
						else
							Ball->_position.y += penetration;
					}
				}
			}
		}
	}

	for (PowerUp &powerUp : this->_powerUps)
	{
		if (!powerUp._destroyed)
		{
			if (powerUp._position.y >= this->_height)
			{
				powerUp._destroyed = GL_TRUE;
			}

			if (checkCollision(*player, powerUp))
			{
				activePowerUp(powerUp);
				powerUp._destroyed = GL_TRUE;
				powerUp._activated = GL_TRUE;
				SoundEngine->play2D("Resource/Music/powerup.wav", false);
			}
		}
	}

	Collision result = checkCollision(*Ball, *player);
	if (!Ball->_stuck && std::get<0>(result))
	{
		GLfloat centerBoard = player->_position.x + player->_size.x / 2;
		GLfloat distance = (Ball->_position.x + Ball->_radius) - centerBoard;
		GLfloat percentage = distance / (player->_size.x / 2);

		GLfloat strength = 2.0f;
		glm::vec2 oldVelocity = Ball->_velocity;
		Ball->_velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		Ball->_velocity = glm::normalize(Ball->_velocity) * glm::length(oldVelocity);

		Ball->_velocity.y = -1 * abs(Ball->_velocity.y);

		Ball->_stuck = Ball->_sticky;

		SoundEngine->play2D("Resource/Music/bleep.wav", false);
	}
}

Direction Game::vectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};

	GLfloat max = 0.0f;
	GLuint bestMatch = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dotProduct = glm::dot(glm::normalize(target), compass[i]);
		if (dotProduct > max)
		{
			max = dotProduct;
			bestMatch = i;
		}
	}

	return (Direction)bestMatch;
}

void Game::resetLevel()
{
	if (this->_level == 0)
		this->_levels[0].load("Level/one.lvl", this->_width, this->_height * 0.5f);
	else if (this->_level == 1)
		this->_levels[1].load("Level/two.lvl", this->_width, this->_height * 0.5f);
	else if (this->_level == 2)
		this->_levels[1].load("Level/three.lvl", this->_width, this->_height * 0.5f);
	else if (this->_level == 3)
		this->_levels[1].load("Level/four.lvl", this->_width, this->_height * 0.5f);

	this->_lives = 5;
}

void Game::resetPlayer()
{
	player->_size = PLAYER_SIZE;
	player->_position = glm::vec2(this->_width / 2 - PLAYER_SIZE.x / 2, this->_height - PLAYER_SIZE.y);
	Ball->reset(player->_position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);

	effects->_chaos = effects->_confuse = GL_FALSE;
	Ball->_passThrough = Ball->_sticky = GL_FALSE;
	player->_color = glm::vec3(1.0f);
	Ball->_color = glm::vec3(1.0f);
}

GLboolean isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type);

void Game::updatePowerUps(GLfloat dt)
{
	for (PowerUp &powerUp : this->_powerUps)
	{
		powerUp._position += powerUp._velocity * dt;

		if (powerUp._activated)
		{
			powerUp._duration -= dt;
			if (powerUp._duration <= 0.0f)
			{
				powerUp._activated = GL_FALSE;

				if (powerUp._type == "sticky")
				{
					if (!isOtherPowerUpActive(this->_powerUps, "sticky"))
					{
						Ball->_sticky = GL_FALSE;
						player->_color = glm::vec3(1.0f);
					}
				}
				else if (powerUp._type == "passThrough")
				{
					if (!isOtherPowerUpActive(this->_powerUps, "passThrough"))
					{
						Ball->_passThrough = GL_FALSE;
						Ball->_color = glm::vec3(1.0f);
					}
				}
				else if (powerUp._type == "confuse")
				{
					if (!isOtherPowerUpActive(this->_powerUps, "confuse"))
					{
						effects->_confuse = GL_FALSE;
					}
				}
				else if (powerUp._type == "chaos")
				{
					if (!isOtherPowerUpActive(this->_powerUps, "chaos"))
					{
						effects->_chaos = GL_FALSE;
					}
				}
			}
		}
	}

	this->_powerUps.erase(std::remove_if(this->_powerUps.begin(), this->_powerUps.end(), 
		[](const PowerUp &powerUp)
	{
		return powerUp._destroyed && !powerUp._activated;
	}), this->_powerUps.end());
}

GLboolean shouldSpawn(GLuint chance)
{
	GLuint random = rand() % chance;
	return random == 0;
}

void Game::spawnPowerUps(GameObject &block)
{
	if (shouldSpawn(75))
	{
		this->_powerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block._position, ResourceManager::getTexture("powerupSpeed")));
	}
	if (shouldSpawn(75))
	{
		this->_powerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block._position, ResourceManager::getTexture("powerupSticky")));
	}
	if (shouldSpawn(75))
	{
		this->_powerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block._position, ResourceManager::getTexture("powerupPassthrough")));
	}
	if (shouldSpawn(75))
	{
		this->_powerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block._position, ResourceManager::getTexture("powerupIncrease")));
	}
	if (shouldSpawn(15))
	{
		this->_powerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block._position, ResourceManager::getTexture("powerupConfuse")));
	}
	if (shouldSpawn(15))
	{
		this->_powerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block._position, ResourceManager::getTexture("powerupChaos")));
	}
}

void activePowerUp(PowerUp &powerUp)
{
	if (powerUp._type == "speed")
	{
		Ball->_velocity *= 1.2;
	}
	else if (powerUp._type == "sticky")
	{
		Ball->_sticky = GL_TRUE;
		player->_color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp._type == "pass-through")
	{
		Ball->_passThrough = GL_TRUE;
		Ball->_color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp._type == "pad-size-increase")
	{
		player->_size.x += 50;
	}
	else if (powerUp._type == "confuse")
	{
		if (!effects->_chaos)
			effects->_confuse = GL_TRUE; 
	}
	else if (powerUp._type == "chaos")
	{
		if (!effects->_confuse)
			effects->_chaos = GL_TRUE;
	}
}

GLboolean isOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
	for (const PowerUp &powerUp : powerUps)
	{
		if (powerUp._activated)
		{
			if (powerUp._type == type)
			{
				return GL_TRUE;
			}
		}
	}

	return GL_FALSE;
}