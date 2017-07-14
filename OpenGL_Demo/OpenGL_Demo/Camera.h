#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Eular Angles
	GLfloat Pitch;
	GLfloat Yaw;

	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset);

	void ProcessMouseScroll(GLfloat yoffset);
private:
	void updateCameraVectors();
};

#endif