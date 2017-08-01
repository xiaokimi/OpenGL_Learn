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

	//Shader floorShader("Shader/AdvanceLight/floor.vs", "Shader/AdvanceLight/floor.frag");
	Shader cubeShader("Shader/AdvanceLight/pointLightCube.vs", "Shader/AdvanceLight/pointLightCube.frag");
	Shader shadowShader("Shader/AdvanceLight/pointLightShadow.vs", "Shader/AdvanceLight/pointLightShadow.frag", "Shader/AdvanceLight/pointLightShadow.geo");

#pragma region "cube vetices"
	GLfloat cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
		// Front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
		// Left face
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		// Right face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		// Top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left  
	};

	GLfloat floorVertices[] = {
		-3.0f, -0.5f, 2.0f, 0.0f, 1.0f, 0.0f, -3.0f, -2.0f,
		3.0f, -0.5f, 2.0f, 0.0f, 1.0f, 0.0f, 3.0f, -2.0f,
		3.0f, -0.5f, -2.0f, 0.0f, 1.0f, 0.0f, 3.0f, 2.0f,

		3.0f, -0.5f, -2.0f, 0.0f, 1.0f, 0.0f, 3.0f, 2.0f,
		-3.0f, -0.5f, -2.0f, 0.0f, 1.0f, 0.0f, -3.0f, 2.0f,
		-3.0f, -0.5f, 2.0f, 0.0f, 1.0f, 0.0f, -3.0f, -2.0f
	};

	GLfloat screenVertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};

#pragma endregion

	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	GLuint cubeTexture = loadTexTure("Resource/Image/container2.png", GL_RGB, GL_RGBA, true);

	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	GLuint depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma region "model init"
	glm::mat4 models[4];
	models[0] = glm::scale(models[0], glm::vec3(10.0f));

	models[1] = glm::translate(models[1], glm::vec3(-2.0f, 2.0f, -2.0f));

	models[2] = glm::rotate(models[2], glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	models[2] = glm::translate(models[2], glm::vec3(3.0f, 0.0f, -1.0f));

	models[3] = glm::translate(models[3], glm::vec3(0.0f, -1.0f, 2.0f));
#pragma endregion

	glm::vec3 lightPos = glm::vec3(0.0f);

	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	GLfloat plane_near = 1.0f;
	GLfloat plane_far = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, plane_near, plane_far);


	glm::vec3 lookTargets[6] = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f)
	};

	glm::vec3 worldUps[6] = {
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f)
	};

	vector<glm::mat4> shadowTransforms;
	for (int i = 0; i < 6; i++)
	{
		glm::mat4 view = glm::lookAt(lightPos, lightPos + lookTargets[i], worldUps[i]);
		shadowTransforms.push_back(shadowProj * view);
	}
	

	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		Controler::getInstance()->update();
		glfwPollEvents();
		Controler::getInstance()->doMovement();

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shadowShader.use();

		for (int i = 0; i < 6; i++)
		{
			char uniformName[20];
			sprintf_s(uniformName, 20, "shadowMatrices[%d]", i);
			shadowShader.setUniformMatrix4fv(uniformName, shadowTransforms[i]);
		}

		shadowShader.setUniform3f("lightPos", lightPos);
		shadowShader.setFloat("far_plane", plane_far);

		for (int i = 0; i < 4; i++)
		{
			shadowShader.setUniformMatrix4fv("model", models[i]);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		glViewport(0, 0, WIDTH, HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeShader.use();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), WIDTH * 1.0f / HEIGHT, 0.1f, 100.0f);

		cubeShader.setUniformMatrix4fv("view", view);
		cubeShader.setUniformMatrix4fv("projection", projection);

		cubeShader.setUniform3f("lightPos", lightPos);
		cubeShader.setUniform3f("viewPos", camera.Position);

		cubeShader.setFloat("far_plane", plane_far);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		cubeShader.setInt("texture1", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		cubeShader.setInt("depthMap", 1);

		for (int i = 0; i < 4; i++)
		{
			cubeShader.setBool("inverse_normal", i == 0);
			cubeShader.setUniformMatrix4fv("model", models[i]);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}