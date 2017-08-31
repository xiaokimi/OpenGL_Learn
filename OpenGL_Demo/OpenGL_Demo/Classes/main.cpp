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
#include <fmod/fmod.hpp>

using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600;

Camera camera;
Game Breakout(static_cast<GLuint>(WIDTH), static_cast<GLuint>(HEIGHT));

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

	//Breakout.init();

	FMOD::System *system;
	FMOD::Sound *sound;
	FMOD::Channel *channel = 0;
	
	void *buff = 0;
	int length = 0;
	
	FMOD_CREATESOUNDEXINFO exinfo;

	FMOD::DSP *dsp;
	float frequency = 0;

	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, 0);
	
	//system->createSound("Resource/Music/breakout.mp3", FMOD_DEFAULT, 0, &sound);
	loadFileMemory("Resource/Music/breakout.mp3", &buff, &length);
	memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.length = length;

	system->createSound(static_cast<const char*>(buff), FMOD_OPENMEMORY, &exinfo, &sound);
	free(buff);


	system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
	dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300);
	dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20);
	system->playSound(sound, 0, true, &channel);

	channel->addDSP(0, dsp);
	channel->setPaused(false);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		Controler::getInstance()->update();
		glfwPollEvents();
		Controler::getInstance()->doMovement();

		GLfloat deltaTime = Controler::getInstance()->getDeltaTime();

		//Breakout.processInput(deltaTime);

		//Breakout.update(deltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Breakout.render();

		glfwSwapBuffers(window);
	}

	sound->release();
	system->close();
	system->release();

	ResourceManager::clear();

	glfwTerminate();
	return 0;
}