#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <cmath>
#include <noise/noise.h>

#include "Block.h"
#include "../core/util/utils.h"

class TileMap
{
private:
	BLOCK_ID * blocks;
	uint width, height, depth;
	uint chunkw, chunkh, chunkd;
	Mesh * chunks;

	void meshNaive();
	Mesh& getChunk(uint x, uint y, uint z);

public:
	TileMap(uint _width, uint _height, uint depth);
	~TileMap();

	void render(glm::mat4 vp, GLuint program);

	BLOCK_ID& get(uint x, uint y, uint z);
};