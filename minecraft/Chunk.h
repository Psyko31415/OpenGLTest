#pragma once
#include <glm.hpp>

#include "Block.h"
#include "../core/render/Sprite.h"

class Chunk
{
private:
	BLOCK_ID * blocks;
	Sprite * sprite;
	HeightType height;

public:
	Chunk();
	~Chunk();

	void init(HeightType _height);
	BLOCK_ID& get(int x, int y, int z);
	void genMesh(float size, GLuint program, glm::vec3 offset);
	void render(glm::mat4 vp);
	HeightType geth();
};

