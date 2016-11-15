#pragma once
#include <cstdlib>
#include <vector>

#include "Mesh.h"

#define cubeVerts 8
#define cubeInds 12
#define HEIGHT_MAP_TILE_RATIO 8

typedef unsigned int uint;
typedef unsigned char HeightType;

float randf();
int randi(int min, int max);
void cube(float width, float greyScale, std::vector<VertexData>& verts, std::vector<IndexData>& inds, glm::vec3 offsetVerts);
