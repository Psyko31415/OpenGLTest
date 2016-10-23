#pragma once
#include "Sprite.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <matrix_transform.hpp>
#include <cstdlib>

class ShapeGen
{
public:
	static Sprite * plane(float width, float depth, float shade, GLuint program);
	static Sprite * pyramid(float width, float height, float depth, GLuint program);
	static Sprite * raceMap(const char * filePath, float grayScale, float width, GLuint program);
};

