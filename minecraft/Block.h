#pragma once
#include "../core/util/utils.h"

typedef unsigned char BLOCK_ID;

class Block
{
private:
	BLOCK_ID id;
	const char * name;
	Float3 colorUpper;
	Float3 colorLower;
	
public:
	Block(BLOCK_ID _id, const char * _name, Float3 _colorUpper, Float3 _colorLower);
	BLOCK_ID getId();
	const char * getName();
	Float3 getUpperColor();
	Float3 getLowerColor();
};

extern Block BLOCK_ARRAY[];

/* THE DEFINES OF YOUR DREAMS */
#define BLOCK_AIR   (BLOCK_ID)0
#define BLOCK_STONE (BLOCK_ID)1
#define BLOCK_GRASS (BLOCK_ID)2
#define BLOCK_RED   (BLOCK_ID)3
