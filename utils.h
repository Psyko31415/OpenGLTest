#pragma once
#include <cstdlib>
#include <vector>

#include "Mesh.h"

#define cubeVerts 8
#define cubeInds 12
#define HEIGHT_MAP_TILE_RATIO 8
#define CHUNK_SIZE 8
#define CHUNK_TO_HEIGHT_MAP_RATIO CHUNK_SIZE / HEIGHT_MAP_TILE_RATIO

typedef unsigned int uint;
typedef unsigned char HeightType;

float randf();
int randi(int min, int max);
void cube(float width, Float3 color, Float3 color2, std::vector<VertexData>& verts, std::vector<IndexData>& inds, glm::vec3 offsetVerts);
