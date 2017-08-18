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

using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600;

Camera camera;

void showFPS(Shader &shader);

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

	// init text
	Text::getInstance()->init();

	Shader shader("Shader/Font/fonts.vs", "Shader/Font/fonts.frag");
	Shader modelShader("Shader/Test/model.vs", "Shader/Test/model.frag");
	Shader cubeShader("Shader/Test/screen.vs", "Shader/Test/screen.frag");

	Model nanosuitModel("Resource/Model/nanosuit/nanosuit.obj");

	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
	shader.use();
	shader.setUniformMatrix4fv("projection", textProjection);

	GLfloat cubeVertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};

	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	GLuint cubeTexture = loadTexTure("Resource/Image/container2.png", GL_RGB, GL_RGBA);

	// model
	glm::mat4 modelMatrix[100];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(-9.0f + i * 2.0f, -2.0f, -9.0f + j * 2.0f));
			model = glm::scale(model, glm::vec3(0.2f));

			modelMatrix[i * 10 + j] = model;
		}
	}

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		Controler::getInstance()->update();
		glfwPollEvents();
		Controler::getInstance()->doMovement();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<GLfloat>(WIDTH) / HEIGHT, 0.1f, 100.0f);

		modelShader.use();

		glm::mat4 model;

		modelShader.setUniformMatrix4fv("view", view);
		modelShader.setUniformMatrix4fv("projection", projection);

		modelShader.setUniform3f("viewPos", camera.Position);
		modelShader.setUniform3f("lightPos", glm::vec3(1.0f));

		nanosuitModel.addInstanceMatrix(modelMatrix, 100);
		nanosuitModel.Draw(modelShader, 100, true);
		/*for (int i = 0; i < 100; i++)
		{
			modelShader.setUniformMatrix4fv("model", modelMatrix[i]);
			nanosuitModel.Draw(modelShader);
		}*/


		showFPS(shader);

		/*cubeShader.use();

		glm::mat4 model;
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.2f));

		cubeShader.setUniformMatrix4fv("model", model);
		cubeShader.setUniformMatrix4fv("view", view);
		cubeShader.setUniformMatrix4fv("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		cubeShader.setInt("ourTexture", 0);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);*/

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void showFPS(Shader &shader)
{
	GLfloat deltaTime = Controler::getInstance()->getDeltaTime();
	GLfloat fps = 1.0f / deltaTime;
	char fpsStr[10];
	sprintf_s(fpsStr, 10, "FPS:%2d", (int)fps);

	Text::getInstance()->RenderText(shader, fpsStr, glm::vec2(25.0f, 25.0f), glm::vec2(0.5f), glm::vec3(1.0f));
}