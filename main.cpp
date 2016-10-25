#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <ctime>
#include <stdio.h>
#include <direct.h>

#include "Sprite.h"
#include "Camera.h"
#include "ShapeGen.h"
#define CD_BUFFER_SIZE 100
const int WIDTH = 1024, HEIGHT = 768;

GLuint createProgram(const char * vertPath, const char * fragPath);
GLuint loadShader(const char * filepath, GLenum type);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

Camera* cam;

int main(int argc, char ** argv)
{
	if (argc >= 2)
	{
		_chdir(argv[1]);
	}
	if (!glfwInit())
	{
		std::cout << "Failed to init glfw" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow * window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "OPENGL PLAYGROUND", NULL, NULL);

	if (!window)
	{
		std::cout << "Window creation failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(window);
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "failed to init glew" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	cam = new Camera((float)WIDTH / HEIGHT, glm::vec3(0, 1, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), window, WIDTH, HEIGHT);

	GLuint program = createProgram("vert1.vert", "frag1.frag");

	Sprite * sprite = ShapeGen::pyramid(1.0f, 3.0f, 1.0f, program);
	Sprite * map = ShapeGen::raceMap("test1.map", 0.5f, 1.5f, program);
	if (map == nullptr)
	{
		return -1;
	}

	float rot = 0.0f;

	int frameCount = 0;
	double startTime = (double)time(0);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window))
	{
		frameCount++;
		if (time(0) - startTime >= 1)
		{
			startTime = (double)time(0);
			printf("FPS: %d\n", frameCount);
			frameCount = 0;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		sprite->rotate(rot, 0, 1, 0);
		sprite->render(cam->getVp());
		map->render(cam->getVp());

		glfwSwapBuffers(window);
		cam->update();
		rot += 0.01f;
	}

	map->cleanup();
	sprite->cleanup();
	delete cam;
	return 0;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	cam->mouse_move(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	cam->key_input(key, action);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	cam->mouse_input(button, action);
}

GLuint createProgram(const char * vertPath, const char * fragPath)
{
	GLuint vert = loadShader(vertPath, GL_VERTEX_SHADER);
	GLuint frag = loadShader(fragPath, GL_FRAGMENT_SHADER);
	GLuint program;

	if (vert && frag)
	{
		program = glCreateProgram();
		glAttachShader(program, vert);
		glAttachShader(program, frag);
		glLinkProgram(program);

		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen)
		{
			char * buffer = new char[infoLen + 1];

			glGetProgramInfoLog(program, infoLen, NULL, buffer);
			buffer[infoLen] = '\0';
			std::cout << buffer << std::endl;

			delete buffer;
		}

		glDetachShader(program, vert);
		glDetachShader(program, frag);

		glDeleteShader(vert);
		glDeleteShader(frag);

		return program;
	}
	else
	{
		std::cout << "something faild when loading shaders" << std::endl;
	}

	return 0;
}

GLuint loadShader(const char * filepath, GLenum type)
{
	GLuint id = glCreateShader(type);

	std::string code;
	std::ifstream stream(filepath);

	if (stream.is_open())
	{
		std::string line = "";
		while (std::getline(stream, line))
		{
			code += line + "\n";
		}
	}
	else
	{
		std::cout << "can't open file " << filepath << std::endl;
		return 0;
	}

	const char * codeCstr = code.c_str();
	glShaderSource(id, 1, &codeCstr, NULL);
	glCompileShader(id);

	GLint infoLen = 0;
	GLint complied = GL_FALSE;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLen);
	glGetShaderiv(id, GL_COMPILE_STATUS, &complied);

	if (!complied)
	{
		char * buffer = new char[infoLen + 1];

		glGetShaderInfoLog(id, infoLen, NULL, buffer);
		std::cout << buffer << std::endl;

		delete buffer;
	}

	return id;
}