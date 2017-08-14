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

	Shader hdrShader("Shader/Texture/HDRcube.vs", "Shader/Texture/HDRcube.frag");
	Shader screenShader("Shader/Texture/screen.vs", "Shader/Texture/screen.frag");
	Shader lightShader("Shader/Texture/light.vs", "Shader/Texture/light.frag");
	Shader blurShader("Shader/Texture/blur.vs", "Shader/Texture/blur.frag");

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

	GLfloat screenVertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f
	};

	GLfloat floorVertices[] = {
		-10.0f, -1.0f, 10.0f, 0.0f, 1.0f, 0.0f, -10.0f, -10.0f,
		10.0f, -1.0f, 10.0f, 0.0f, 1.0f, 0.0f, 10.0f, -10.0f,
		10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f,

		10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f,
		-10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f, -10.0f, 10.0f,
		-10.0f, -1.0f, 10.0f, 0.0f, 1.0f, 0.0f, -10.0f, -10.0f
	};

	glm::vec3 positions[6] = {
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),

		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f)
	};

	glm::vec2 texCoords[6] = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f)
	};

	glm::vec3 tangents[2], bitangents[2];
	calcTangent(positions, texCoords, tangents[0], bitangents[0]);
	calcTangent(positions + 3, texCoords + 3, tangents[1], bitangents[1]);

	glm::vec3 normals = glm::vec3(0.0f, 0.0f, 1.0f);

	GLfloat wallVertices[14 * 6] = { 0.0f };

	for (int i = 0; i < 6; i++)
	{
		// position
		wallVertices[i * 14 + 0] = positions[i].x;
		wallVertices[i * 14 + 1] = positions[i].y;
		wallVertices[i * 14 + 2] = positions[i].z;

		// texCoord
		wallVertices[i * 14 + 3] = texCoords[i].x;
		wallVertices[i * 14 + 4] = texCoords[i].y;

		// normal
		wallVertices[i * 14 + 5] = normals.x;
		wallVertices[i * 14 + 6] = normals.y;
		wallVertices[i * 14 + 7] = normals.z;

		// tangent
		wallVertices[i * 14 + 8] = tangents[i < 3 ? 0 : 1].x;
		wallVertices[i * 14 + 9] = tangents[i < 3 ? 0 : 1].y;
		wallVertices[i * 14 + 10] = tangents[i < 3 ? 0 : 1].z;

		// bitangent
		wallVertices[i * 14 + 11] = bitangents[i < 3 ? 0 : 1].x;
		wallVertices[i * 14 + 12] = bitangents[i < 3 ? 0 : 1].y;
		wallVertices[i * 14 + 13] = bitangents[i < 3 ? 0 : 1].z;
	}

#pragma endregion

	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	GLuint floorVAO, floorVBO;
	glGenVertexArrays(1, &floorVAO);
	glBindVertexArray(floorVAO);

	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	GLuint screenVAO, screenVBO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);

	glGenBuffers(1, &screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	GLuint floorTexture = loadTexTure("Resource/Image/wood.png", GL_RGB, GL_RGB);
	GLuint cubeTexture = loadTexTure("Resource/Image/container2.png", GL_RGB, GL_RGBA);

	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	GLuint textureColor[2];
	glGenTextures(2, textureColor);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, textureColor[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textureColor[i], 0);
	}
	

	GLuint RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER != GL_FRAMEBUFFER_COMPLETE))
	{
		cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint pingpongFBO[2];
	glGenFramebuffers(2, pingpongFBO);
	
	GLuint pingpongColorbuffers[2];
	glGenTextures(2, pingpongColorbuffers);

	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!" << endl;
		}
	}

	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	lightPositions.push_back(glm::vec3(-0.8f, 2.4f, -1.0f));
	// colors
	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(2.0f, 2.0f, 2.0f));
	lightColors.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 1.5f));
	lightColors.push_back(glm::vec3(0.0f, 1.5f, 0.0f));

	GLfloat exposure = 1.0f;

	//glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		Controler::getInstance()->update();
		glfwPollEvents();
		Controler::getInstance()->doMovement();

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			if (exposure > 0.0f)
				exposure -= 0.01f;
			else
				exposure = 0.0f;
		}
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			if (exposure < 10.0f)
				exposure += 0.01f;
			else
				exposure = 1.0f;
		}


		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), WIDTH * 1.0f / HEIGHT, 0.1f, 100.0f);

		hdrShader.use();
		hdrShader.setUniformMatrix4fv("view", view);
		hdrShader.setUniformMatrix4fv("projection", projection);

		hdrShader.setUniform3f("viewPos", camera.Position);

		for (int i = 0; i < 4; i++)
		{
			char lightPos[20];
			char lightColor[20];

			sprintf_s(lightPos, 20, "lightPositions[%d]", i);
			sprintf_s(lightColor, 20, "lightColors[%d]", i);

			hdrShader.setUniform3f(lightPos, lightPositions[i]);
			hdrShader.setUniform3f(lightColor, lightColors[i]);
		}

		// floor
		glm::mat4 model;
		hdrShader.setUniformMatrix4fv("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		hdrShader.setInt("textureMap", 0);

		glBindVertexArray(floorVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// cube
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		hdrShader.setInt("textureMap", 0);

		// cube1
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		hdrShader.setUniformMatrix4fv("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// cube2
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		hdrShader.setUniformMatrix4fv("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// cube3
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		hdrShader.setUniformMatrix4fv("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// cube4
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
		model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(1.25));
		hdrShader.setUniformMatrix4fv("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// cube5
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
		model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		hdrShader.setUniformMatrix4fv("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// cube6
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f)); 
		hdrShader.setUniformMatrix4fv("model", model);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// lighting
		lightShader.use();
		lightShader.setUniformMatrix4fv("view", view);
		lightShader.setUniformMatrix4fv("projection", projection);

		for (int i = 0; i < 4; i++)
		{
			glm::mat4 lightModel;
			lightModel = glm::translate(lightModel, lightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));

			lightShader.setUniformMatrix4fv("model", lightModel);

			lightShader.setUniform3f("lightColor", lightColors[i]);

			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		bool horizontal = true;
		unsigned int amount = 10;
		blurShader.use();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i % 2]);

			blurShader.setInt("horizontal", horizontal);

			glBindTexture(GL_TEXTURE_2D, i == 0 ? textureColor[1] : pingpongColorbuffers[(i+1) % 2]); 
			
			glBindVertexArray(screenVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			horizontal = !horizontal;
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		screenShader.use();

		screenShader.setFloat("exposure", exposure);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureColor[0]);
		screenShader.setInt("textureMap", 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
		screenShader.setInt("hdrMap", 1);

		screenShader.setBool("bloom", Controler::getInstance()->getKeyState(GLFW_KEY_H));

		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}