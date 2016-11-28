#include "TileMap.h"

TileMap::TileMap(uint _width, uint _height, uint _depth) : width(_width), height(_height), depth(_depth), chunkw(_width / CHUNK_SIZE), chunkh(_height / CHUNK_SIZE), chunkd(_depth / CHUNK_SIZE)
{
	blocks = new BLOCK_ID[width * height * depth];
	chunks = new Mesh[chunkw * chunkh * chunkd];
	memset(blocks, 0, width * height * depth);
	noise::module::Perlin noiseGen;
	noiseGen.SetOctaveCount(1);
	for (uint x = 0; x < width; x++)
	{
		for (uint z = 0; z < depth; z++)
		{
			int y = (int)(height * (1 + noiseGen.GetValue(x / 100.0f, 3.8, z / 100.0f))) / 2;
			get(x, y, z) = BLOCK_GRASS;
			for (uint i = 0; i < y; i++)
			{
				get(x, i, z) = BLOCK_STONE;
			}
		}
	}
	meshNaive();
}

TileMap::~TileMap()
{
	uint chunkCount = chunkw * chunkh * chunkd;
	for (uint i = 0; i < chunkCount; i++)
	{
		chunks[i].cleanup();
	}
	delete[] chunks;
	delete[] blocks;
}

void TileMap::meshNaive()
{
	for (uint cx = 0; cx < chunkw; cx++)
	{
		for (uint cy = 0; cy < chunkh; cy++)
		{
			for (uint cz = 0; cz < chunkd; cz++)
			{
				VertexData * verts = new VertexData[cubeVerts * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
				IndexData * inds = new IndexData[cubeInds * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
				GLushort offsetInds = 0;
				int counter = 0;

				for (uint x = 0; x < CHUNK_SIZE; x++)
				{
					for (uint y = 0; y < CHUNK_SIZE; y++)
					{
						for (uint z = 0; z < CHUNK_SIZE; z++)
						{
							BLOCK_ID blockId = get(cx * CHUNK_SIZE + x, cy * CHUNK_SIZE + y, cz * CHUNK_SIZE + z);
							if (blockId != BLOCK_AIR)
							{
								Block block = BLOCK_ARRAY[blockId];
								cube(1.0f, block.getUpperColor(), block.getLowerColor(), verts + counter * cubeVerts, inds + counter * cubeInds, glm::vec3(cx * CHUNK_SIZE + x, cy * CHUNK_SIZE + y, cz * CHUNK_SIZE + z), offsetInds);
								offsetInds += cubeVerts;
								counter++;
							}
						}
					}
				}
				getChunk(cx, cy, cz).setData(counter * cubeVerts, verts, counter * cubeInds, inds);
			}
		}
	}
}

BLOCK_ID& TileMap::get(uint x, uint y, uint z)
{
	return blocks[x + y * width + z * width * height];
}

Mesh& TileMap::getChunk(uint x, uint y, uint z)
{
	return chunks[x + y * chunkw + z * chunkw * chunkh];
}

void TileMap::render(glm::mat4 vp, GLuint program)
{
	uint chunkCount = chunkw * chunkh * chunkd;
	for (uint i = 0; i < chunkCount; i++)
	{
		chunks[i].render(vp, program);
	}
}
