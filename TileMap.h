#pragma once

#include <string>
#include <vector>

#include "Sprite.h"
#include "utils.h"
#include "HeighMap.h"

typedef enum 
{
	TILE_EMPTY = 0,
	TILE_SOLID = 1
} Tile;

typedef struct 
{
	uint x, y, z;
} TileMapPos;

class TileMap
{
private:
	uint width, height, depth;
	Tile * tiles;
	Sprite * sprite;

	Tile& get(TileMapPos pos);

public:
	TileMap(uint _width, uint _height, uint _depth, float size, float greyScale, GLuint program);
	~TileMap();

	Tile& operator[](TileMapPos pos);

	uint getw();
	uint geth();
	uint getd();

	void render(glm::mat4 vp);
};

