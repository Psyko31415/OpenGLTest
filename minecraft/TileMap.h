#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <cmath>
#include <noise/noise.h>

#include "Block.h"
#include "../core/util/utils.h"

#define cubeNeighbours 6

typedef struct
{
	int dx, dy, dz;	
} BlockNeighbourData;

class TileMap
{
private:
	BLOCK_ID * blocks;
	int width, height, depth;
	int chunkw, chunkh, chunkd;
	Mesh * chunks;

	void meshNaive();
	void mesh();
	Mesh& getChunk(int x, int y, int z);

public:
	TileMap(int _width, int _height, int depth);
	~TileMap();

	bool inside(int x, int y, int z);
	void render(glm::mat4 vp, GLuint program);

	BLOCK_ID& get(int x, int y, int z);
};