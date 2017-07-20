#ifndef __CONTROLER_H__
#define __CONTROLER_H__

#include <GLFW/glfw3.h>
#include "Camera.h"

class Controler
{
public:
	enum MouseState
	{
		MouseState_NONE = 0,
		MouseState_PRESS = 1,
		MouseState_MOVE = 2,
	};

	static Controler *getInstance();

	Controler();
	~Controler();

	void init(GLFWwindow *window, Camera *camera);

	void update();

	void doMovement();

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
	void mouseButtonCallback(GLFWwindow *window, int button, int action, int mode);
	void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
	void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

private:
	bool _keys[1024];
	Camera *_camera;

	MouseState _state;
	double _lastX;
	double _lastY;

	GLfloat _deltaTime;
	GLfloat _lastFrame;
};

#endif