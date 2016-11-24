#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <matrix_transform.hpp>
#include <cstdlib>

#include "../render/Mesh.h"
#include "../render/Sprite.h"

typedef unsigned int uint;

class ShapeGen
{
public:
	static Sprite * plane(float width, float depth, float shade, GLuint program);
	static Sprite * pyramid(float width, float height, float depth, GLuint program);
};

