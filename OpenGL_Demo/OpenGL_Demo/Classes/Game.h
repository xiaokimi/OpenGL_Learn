#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <tuple>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameLevel.h"
#include "BallObject.h"
#include "PowerUp.h"

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction
{
	UP,
	Right,
	DOWN,
	Left
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -250.0f);
const GLfloat BALL_RADIUS = 12.5f;

class Game
{
public:
	Game(GLuint width, GLuint height);
	~Game();

	void init();

	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();

	void keyCallback(int key, int scancode, int action, int mode);

	GLboolean checkCollision(GameObject &one, GameObject &two);

	Collision checkCollision(BallObject &ball, GameObject &obj);

	void doCollision();

	Direction vectorDirection(glm::vec2 target);

	void resetLevel();
	void resetPlayer();

	void spawnPowerUps(GameObject &block);
	void updatePowerUps(GLfloat dt);

public:
	GLuint _lives;

private:
	GameState _state;
	GLboolean _keys[1024];
	GLboolean _keyProcessed[1024];
	GLuint _width, _height;

	std::vector<GameLevel> _levels;
	GLuint _level;

	std::vector<PowerUp> _powerUps;
};

#endif