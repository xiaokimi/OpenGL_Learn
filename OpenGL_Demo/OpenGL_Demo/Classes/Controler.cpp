#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "stb_image.h"

#include "Controler.h"

using namespace std;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mode);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void set_custom_cursor(GLFWwindow *window, const char *cursorPath);

Controler *_controler = nullptr;

Controler *Controler::getInstance()
{
	if (!_controler)
	{
		_controler = new Controler();
	}

	return _controler;
}

Controler::Controler()
: _state(MouseState_NONE)
, _lastX(0.0f)
, _lastY(0.0f)
, _deltaTime(0.0f)
, _lastFrame(0.0f)
{

}

Controler::~Controler()
{
	if (_controler)
	{
		delete _controler;
	}
}

void Controler::init(GLFWwindow *window, Camera *camera)
{
	_camera = camera;

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void Controler::update()
{
	GLfloat currentFrame = glfwGetTime();
	_deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
}

void Controler::doMovement()
{
	if (_keys[GLFW_KEY_W])
		_camera->ProcessKeyboard(FORWARD, _deltaTime);
	if (_keys[GLFW_KEY_S])
		_camera->ProcessKeyboard(BACKWARD, _deltaTime);
	if (_keys[GLFW_KEY_A])
		_camera->ProcessKeyboard(LEFT, _deltaTime);
	if (_keys[GLFW_KEY_D])
		_camera->ProcessKeyboard(RIGHT, _deltaTime);
}

void Controler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			_keys[key] = true;
		else if (action == GLFW_RELEASE)
			_keys[key] = false;
	}
}

void Controler::mouseButtonCallback(GLFWwindow *window, int button, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			cout << "mouse left button pressed" << endl;
			break;

		case GLFW_MOUSE_BUTTON_MIDDLE:
			cout << "mouse middle button pressed" << endl;
			break;

		case GLFW_MOUSE_BUTTON_RIGHT:

			glfwGetCursorPos(window, &_lastX, &_lastY);
			_state = MouseState_PRESS;
			break;

		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:

			break;

		case GLFW_MOUSE_BUTTON_MIDDLE:

			break;

		case GLFW_MOUSE_BUTTON_RIGHT:

			_state = MouseState_NONE;
			break;

		default:
			break;
		}
	}
}

void Controler::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
	if (_state == MouseState_PRESS)
	{
		GLfloat xoffset = xpos - _lastX;
		GLfloat yoffset = _lastY - ypos;		// 屏幕坐标原点是左上

		_lastX = xpos;
		_lastY = ypos;

		_camera->ProcessMouseMovement(xoffset, yoffset);
	}
}

void Controler::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	_camera->ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	Controler::getInstance()->keyCallback(window, key, scancode, action, mode);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mode)
{
	Controler::getInstance()->mouseButtonCallback(window, button, action, mode);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	Controler::getInstance()->cursorPositionCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	Controler::getInstance()->scrollCallback(window, xoffset, yoffset);
}

void set_custom_cursor(GLFWwindow *window, const char *cursorPath)
{
	int nWidth, nHeight, nrChannels;
	stbi_set_flip_vertically_on_load(false);
	unsigned char *data = stbi_load(cursorPath, &nWidth, &nHeight, &nrChannels, 0);
	if (data)
	{
		GLFWimage *image = new GLFWimage();
		image->width = nWidth;
		image->height = nHeight;
		image->pixels = data;

		GLFWcursor *cursor = glfwCreateCursor(image, 0, 0);
		glfwSetCursor(window, cursor);
		delete image;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}