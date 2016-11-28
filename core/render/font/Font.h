#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <glm.hpp>
#include <map>
#include <cmath>

typedef struct
{
	GLuint textureId;
	glm::vec2 size;
	glm::vec2 bearing;
	GLuint advance;
} Character;

class Font
{
private:
	FT_Face face;
	std::map<char, Character> characters;
	GLuint vao, vbo;
public:
	Font(const char * fontPath, int size, FT_Library ft);
	~Font();
	void renderText(GLuint program, const char * text, float x, float y, float scale, glm::vec4 color, glm::mat4 proj);
	void renderFloat(GLuint program, float value, float x, float y, float scale, glm::vec4 color, glm::mat4 proj);
	void renderVec3(GLuint program, glm::vec3 value, float x, float y, float scale, glm::vec4 color, glm::mat4 proj);

};

