#include "Camera.h"

#define scalar 0.1f
#define mouseSens 0.002f

Camera::Camera(float ar, glm::vec3 _up, glm::vec3 _lookDir, glm::vec3 _pos, GLFWwindow* _window, int w, int h) : upvec(_up), pos(_pos), window(_window), width(w), height(h)
{
	projection = glm::perspective(glm::radians(45.0f), ar, 0.1f, 1000.0f);
	lookDir = glm::normalize(_lookDir);
	updateView();
	up	  = false;
	down  = false;
	left  = false;
	right = false;
}

Camera::~Camera()
{
}

void Camera::key_input(int key, int action)
{
	if (key == GLFW_KEY_A)
	{
		left = (bool)action;
	}
	else if (key == GLFW_KEY_D)
	{
		right = (bool)action;
	}
	else if (key == GLFW_KEY_W)
	{
		up = (bool)action;
	}
	else if (key == GLFW_KEY_S)
	{
		down = (bool)action;
	}
}

void Camera::mouse_input(int button, int action) 
{
}

void Camera::mouse_move(double x, double y)
{
	glm::vec2 pos((float)x, (float)y);
	glm::vec2 delta = pos;

	float verticalAngle = delta.y * mouseSens;
	float horizontalAngle = delta.x * mouseSens;

	lookDir = glm::normalize(glm::vec3(
		-cos(verticalAngle) * sin(horizontalAngle),
		sin(-verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle))
	);
}

void Camera::update()
{
	if (up)
	{
		pos += lookDir * scalar;
	}
	if (down)
	{
		pos -= lookDir * scalar;
	}
	if (left)
	{
		pos += glm::normalize(glm::cross(upvec, lookDir)) * scalar;
	}
	if (right)
	{
		pos += glm::normalize(glm::cross(upvec, lookDir)) * scalar * -1.0f;
	}
	updateView();
}

void Camera::updateView()
{
	view = glm::lookAt(pos, pos + lookDir, upvec);
}


glm::mat4 Camera::getView()
{
	return view;
}

glm::mat4 Camera::getProjection()
{
	return projection;
}

glm::mat4 Camera::getVp()
{
	return projection * view;
}

glm::vec3 Camera::getPos()
{
	return pos;
}