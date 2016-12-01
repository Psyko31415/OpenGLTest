#pragma once
#include <cstdlib>
#include <vector>

#include "../render/Mesh.h"

#define cubeVerts 8
#define cubeInds 12
#define cubeSides 6
#define HEIGHT_MAP_TILE_RATIO 8
#define CHUNK_SIZE 16
#define CHUNK_TO_HEIGHT_MAP_RATIO CHUNK_SIZE / HEIGHT_MAP_TILE_RATIO

typedef unsigned int uint;
typedef unsigned char HeightType;

float randf();
int randi(int min, int max);
void cube(float width, Float3 color, Float3 color2, std::vector<VertexData>& verts, std::vector<IndexData>& inds, glm::vec3 offsetVerts);
void cube(float width, Float3 color, Float3 color2, VertexData * verts, IndexData * inds, glm::vec3 offsetVerts, GLushort offsetInds);
void partCube(float width, unsigned char sides, VertexData * verts, IndexData * inds, int& vertOffset, int& indsOffset, glm::vec3 pos, Float3 upperColor, Float3 lowerColor);