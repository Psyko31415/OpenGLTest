#include <iostream>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <direct.h>
#include <Windows.h>
#include <time.h>
#include <cstdlib>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <matrix_transform.hpp>

#include "shader.h"
#include "Sprite.h"
#include "Camera.h"
#include "ShapeGen.h"
#include "TileMap.h"
#include "Font.h"

float WIDTH = 1024.0f, HEIGHT = 768.0f;

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
		return 1;
	}

	FT_Library ft;

	if (FT_Init_FreeType(&ft)) 
	{
		std::cout << "Could not init freetype library" << std::endl;
		return 1;
	}

	srand((unsigned)time(0));

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow * window;
	window = glfwCreateWindow((int)WIDTH, (int)HEIGHT, "!Minecraft", NULL, NULL);

	if (!window)
	{
		std::cout << "Window creation failed" << std::endl;
		glfwTerminate();
		return 1;
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
		return 1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint textRenderingProgram = createProgram("text.vert", "text.frag");
	glm::mat4 textRenderingProjection = glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT);

	GLuint program = createProgram("vert1.vert", "frag1.frag");

	cam = new Camera((float)WIDTH / HEIGHT, glm::vec3(0, 1, 0), glm::normalize(glm::vec3(1, 1, 1)), glm::vec3(0, 0, 0), window, WIDTH, HEIGHT);
	Font testFont("consola.ttf", 16, ft);

	Sprite * sprite = ShapeGen::pyramid(1.0f, 3.0f, 1.0f, program);
	TileMap tm(2, 16, 2, 1.0f, program);
	
	float rot = 0.0f;

	SYSTEMTIME st;
	GetSystemTime(&st);
	double fps = 60;
	double startTime = st.wMilliseconds + st.wSecond * 1000;
	double targetTime = 1000.0 / fps;
	int frameCount = 0;
	double timePassed = 0;
	

	glm::vec4 red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window))
	{
		frameCount++;
		GetSystemTime(&st);
		double currentTime = st.wMilliseconds + st.wSecond * 1000;
		double elapsed = currentTime - startTime;
		startTime = currentTime;
		timePassed += elapsed;

		if (timePassed > 1000)
		{
			// std::cout << "FPS: " << frameCount << std::endl;
			frameCount = 0;
			timePassed -= 1000;
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		sprite->rotate(rot, 0, 1, 0);
		sprite->render(cam->getVp());
		tm.render(cam->getVp());

		testFont.renderText(textRenderingProgram, "Hej mamma, LOOK IM RENDERED", 300, 300, 1.0f, red, textRenderingProjection);

		glfwSwapBuffers(window);
		cam->update();
		rot += 0.01f;

	}

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