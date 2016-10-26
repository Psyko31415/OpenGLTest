#pragma once
#include "Sprite.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <matrix_transform.hpp>
#include <cstdlib>

Sprite * raceMap2(const char * filePath, float grayScale, GLuint program);
