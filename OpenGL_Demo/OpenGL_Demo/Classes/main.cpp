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

using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600;

Camera camera;

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

	Shader planetShader("Shader/GLSL/planet.vs", "Shader/GLSL/planet.frag");
	Shader rockShader("Shader/GLSL/rock.vs", "Shader/GLSL/rock.frag");

	Model planetModel("Resource/Model/planet/planet.obj");
	Model rockModel("Resource/Model/rock/rock.obj");

	// 
	srand(glfwGetTime());
	int rockAmount = 10000;
	glm::mat4 *rockModelMatrix = new glm::mat4[rockAmount];

	float radius = 50.0f;
	float offset = 5.0f;
	for (int i = 0; i < rockAmount; i++)
	{
		glm::mat4 model;
		
		GLfloat angle = glm::radians((GLfloat)i / (GLfloat)rockAmount * 360.0f);
		GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat x = sin(angle) * radius + displacement;
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat y = displacement * 0.4f; 
		displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
		GLfloat z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		
		GLfloat scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));
		
		GLfloat rotAngle = glm::radians((rand() % 360) * 1.0f);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		
		rockModelMatrix[i] = model;
	}

	float a = sin(glm::radians(90.0f));

	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		Controler::getInstance()->update();
		glfwPollEvents();
		Controler::getInstance()->doMovement();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model;
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, WIDTH * 1.0f / HEIGHT, 0.1f, 100.0f);

		planetShader.use();
		planetShader.setUniformMatrix4fv("model", model);
		planetShader.setUniformMatrix4fv("view", view);
		planetShader.setUniformMatrix4fv("projection", projection);

		planetModel.Draw(planetShader);

		rockShader.use();
		rockShader.setUniformMatrix4fv("view", view);
		rockShader.setUniformMatrix4fv("projection", projection);

		rockModel.addInstanceMatrix(rockModelMatrix, rockAmount);
		rockModel.Draw(rockShader, rockAmount, true);

		glfwSwapBuffers(window);
	}

	delete[] rockModelMatrix;

	glfwTerminate();
	return 0;
}