#include <iostream>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// std_image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Controler.h"
#include "model.h"
#include "ThirdFunc.h"
#include "Text.h"
#include "Game.h"
#include "ResourceManager.h"
#include "AudioManager.h"

#include <Box2D/Box2D.h>

using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600;

Camera camera;
Game Breakout(static_cast<GLuint>(WIDTH), static_cast<GLuint>(HEIGHT));

void showFPS(float dt);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	Controler::getInstance()->init(window, &camera);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Breakout.init();

	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body *groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body *body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 3.0f;
	body->CreateFixture(&fixtureDef);

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;


	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float dt = 0.0f;
	float total = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		Controler::getInstance()->update();
		glfwPollEvents();
		Controler::getInstance()->doMovement();

		GLfloat deltaTime = Controler::getInstance()->getDeltaTime();
		dt += deltaTime;
		if (dt >= timeStep)
		{
			dt -= timeStep;
			total += timeStep;
			world.Step(timeStep, velocityIterations, positionIterations);
			b2Vec2 position = body->GetPosition();
			cout << "----------------------" << endl;
			printf("%4.5f, %4.5f\n", position.x, position.y);

			double tmpy = 4.0 - 5 * (total * total);
			printf("%4.5f\n", tmpy);
			cout << "----------------------" << endl;
		}


		//Breakout.processInput(deltaTime);

		//Breakout.update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Breakout.render();

		// Ë¢ÐÂÂÊ
		showFPS(deltaTime);

		glfwSwapBuffers(window);
	}

	ResourceManager::clear();

	glfwTerminate();
	return 0;
}

void showFPS(float dt)
{
	char str[20];
	sprintf_s(str, 20, "FPS:%d", (int)(1.0 / dt));
	Text::getInstance()->RenderText(str, glm::vec2(5.0f, 5.0f), glm::vec2(0.5f), glm::vec3(1.0f));
}