#include "Camera.h"

Camera::Camera(glm::vec3 position /* = glm::vec3(0.0f , 0.0f, 3.0f) */)
{
	Position = position;
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	
	Pitch = 0.0f;
	Yaw = 0.0f;

	MovementSpeed = 3.0f;
	MouseSensitivity = 0.05f;
	Zoom = 45.0f;

	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, WorldUp);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Pitch += yoffset;
	Yaw += xoffset;

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;

	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;

	// Pitch
	front.y = sin(glm::radians(Pitch));
	front.x = cos(glm::radians(Pitch));
	front.z = cos(glm::radians(Pitch));

	// Yaw
	front.x = sin(glm::radians(Yaw));
	front.z = -cos(glm::radians(Yaw));

	Front = glm::normalize(front);

	Right = glm::cross(Front, WorldUp);
	Up = glm::cross(Right, Front);
}