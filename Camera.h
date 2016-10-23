#pragma once
#include <glm.hpp>
#include <glfw3.h>
#include <matrix_transform.hpp>

class Camera
{
private:
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 upvec;
	glm::vec3 pos;
	glm::vec3 lookDir;
	GLFWwindow* window;
	int width, height;
	bool up, down, left, right;

	void updateView();

public:
	Camera(float ar, glm::vec3 _up, glm::vec3 _lookDir, glm::vec3 _pos, GLFWwindow* _window, int w, int h);
	~Camera();

	void key_input(int key, int action);
	void mouse_input(int button, int action);
	void mouse_move(double x, double y);
	void update();

	glm::mat4 getView();
	glm::mat4 getVp();
	glm::mat4 getProjection();
};

