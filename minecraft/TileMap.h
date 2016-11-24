#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <cmath>

#include "utils.h"
#include "HeighMap.h"
#include "Chunk.h"

class TileMap
{
private:
	Chunk * chunks;
	uint chunkw, chunkd, arrsize;
	HeightType chunkh;
	HeightMap hm;

public:
	TileMap(uint _chunkw, HeightType _chunkh, uint _chunkd, float blocksize, GLuint program);
	~TileMap();

	void render(glm::mat4 vp);
	void genMesh(float size, GLuint program);
	Chunk& get(uint x, uint z);
	int distToCenterOf(uint x, uint z, uint chunkx, uint chunkz);
};

