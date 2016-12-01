#include "Block.h"

Block BLOCK_ARRAY[] = {					 
	Block(BLOCK_AIR,   "air",   { 0.0f,  0.0f,  0.0f  }, { 0.0f,  0.0f,  0.0f  }),
	Block(BLOCK_STONE, "stone", { 0.5f,  0.5f,  0.5f  }, { 0.3f,  0.3f,  0.3f  }),
	Block(BLOCK_GRASS, "grass", { 0.0f,  1.0f,  0.0f  }, { 0.0f,  0.5f,  0.0f  }),
	Block(BLOCK_RED,   "red",   { 1.0f,  0.0f,  0.0f  }, { 0.5f,  0.0f,  0.0f  }),
	Block(BLOCK_WOOD,  "wood",  { 0.55f, 0.27f, 0.07f }, { 0.45f, 0.17f, 0.02f }),
	Block(BLOCK_LEAF , "leaf",  { 0.0f,  0.6f,  0.0f },  { 0.0f,  0.3f,  0.0f }),

};

Block::Block(BLOCK_ID _id, const char * _name, Float3 _colorUpper, Float3 _colorLower) : id(_id), name(_name), colorUpper(_colorUpper), colorLower(_colorLower) {}

BLOCK_ID Block::getId()
{
	return id;
}
const char * Block::getName()
{
	return name;
}
Float3 Block::getUpperColor() 
{
	return colorUpper;
}
Float3 Block::getLowerColor()
{
	return colorLower;
}