#pragma once
#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <string>

GLuint createProgram(const char * vertPath, const char * fragPath);
GLuint loadShader(const char * filepath, GLenum type);
