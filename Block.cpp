#include "Block.h"

Block BLOCK_ARRAY[] = {
	Block(0, "air",   { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }),
	Block(1, "stone", { 0.5f, 0.5f, 0.5f }, { 0.3f, 0.3f, 0.3f }),
	Block(2, "grass", { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.5f, 0.0f }),
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