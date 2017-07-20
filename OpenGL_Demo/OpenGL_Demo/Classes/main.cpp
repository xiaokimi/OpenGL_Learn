#include <iostream>

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

	Shader cubeShader("Shader/Advance/cube.vs", "Shader/Advance/cube.frag");
	Shader mirrorShader("Shader/Advance/mirror.vs", "Shader/Advance/mirror.frag");

	GLfloat cubeVertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	GLfloat mirrorVertices[] = {
		-3.0f, 0.0f, 3.0f,
		3.0f, 0.0f, 3.0f,
		3.0f, 0.0f, -3.0f,

		-3.0f, 0.0f, 3.0f,
		3.0f, 0.0f, -3.0f,
		-3.0f, 0.0f, -3.0f
	};

	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	GLuint mirrorVAO, mirrorVBO;
	glGenVertexArrays(1, &mirrorVAO);
	glBindVertexArray(mirrorVAO);

	glGenBuffers(1, &mirrorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mirrorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mirrorVertices), mirrorVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


	GLuint cubeTexture;
	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int nWidth, nHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("Resource/Image/container2.png", &nWidth, &nHeight, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		Controler::getInstance()->update();
		glfwPollEvents();
		Controler::getInstance()->doMovement();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glStencilMask(0xff);
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		mirrorShader.use();

		glm::mat4 mirrorModel;
		mirrorModel = glm::translate(mirrorModel, glm::vec3(0.0f, -1.0f, 0.0f));

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, WIDTH * 1.0f / HEIGHT, 0.1f, 100.0f);

		mirrorShader.setUniformMatrix4fv("model", mirrorModel);
		mirrorShader.setUniformMatrix4fv("view", view);
		mirrorShader.setUniformMatrix4fv("projection", projection);

		glBindVertexArray(mirrorVAO);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);

		glStencilMask(0x00);
		glStencilFunc(GL_EQUAL, 1, 0xff);
		cubeShader.use();

		glm::mat4 cubeModel;
		cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, -2.0f, 0.0f));
		cubeModel = glm::scale(cubeModel, glm::vec3(1.0f, -1.0f, 1.0f));

		cubeShader.setUniformMatrix4fv("model", cubeModel);
		cubeShader.setUniformMatrix4fv("view", view);
		cubeShader.setUniformMatrix4fv("projection", projection);

		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glStencilMask(0x00);
		glStencilFunc(GL_ALWAYS, 1, 0xff);

		cubeModel = glm::mat4();
		cubeShader.setUniformMatrix4fv("model", cubeModel);

		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		glStencilMask(0xff);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	glfwTerminate();
	return 0;
}