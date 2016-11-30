#include "TileMap.h"
BlockNeighbourData cubeSideNeighbours[cubeNeighbours] =
{
	{  0,  1,  0 },
	{  0,  0,  1 },
	{  1,  0,  0 },
	{  0,  0, -1 },
	{ -1,  0,  0 },
	{  0, -1,  0 }
};

TileMap::TileMap(int _width, int _height, int _depth, int seed) : width(_width), height(_height), depth(_depth), chunkw(_width / CHUNK_SIZE), chunkh(_height / CHUNK_SIZE), chunkd(_depth / CHUNK_SIZE)
{
	blocks = new BLOCK_ID[width * height * depth];
	chunks = new Mesh[chunkw * chunkh * chunkd];
	memset(blocks, 0, width * height * depth);

	noise::module::Perlin noiseGen;
	noiseGen.SetOctaveCount(4);
	noiseGen.SetSeed(seed);

	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < depth; z++)
		{
			float y = ((height) * (1 + noiseGen.GetValue(x / 100.0f, 1, z / 100.0f)) / 2);
			y = (int)(y * y / height);
			get(x, y, z) = BLOCK_GRASS;
			for (int i = 0; i < y; i++)
			{
				get(x, i, z) = BLOCK_STONE;
			}
		}
	}
	mesh();
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

void TileMap::mesh()
{
	for (int cx = 0; cx < chunkw; cx++)
	{
		for (int cy = 0; cy < chunkh; cy++)
		{
			for (int cz = 0; cz < chunkd; cz++)
			{
				VertexData * verts = new VertexData[cubeVerts * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
				IndexData * inds = new IndexData[cubeInds * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
				int vertsOffset = 0, indsOffset = 0;
				unsigned char cubeVertsBitMap = 0;

				for (int x = 0; x < CHUNK_SIZE; x++)
				{
					for (int y = 0; y < CHUNK_SIZE; y++)
					{
						for (int z = 0; z < CHUNK_SIZE; z++)
						{
							int currx = cx * CHUNK_SIZE + x, curry = cy * CHUNK_SIZE + y, currz = cz * CHUNK_SIZE + z;
							BLOCK_ID blockId = get(currx, curry, currz);
							if (blockId != BLOCK_AIR)
							{
								unsigned char currCubeSides = 0;
								for (int i = 0; i < cubeSides; i++)
								{
									int nextx = currx + cubeSideNeighbours[i].dx, nexty = curry + cubeSideNeighbours[i].dy, nextz = currz + cubeSideNeighbours[i].dz;
									if (inside(nextx, nexty, nextz) && get(nextx, nexty, nextz) != BLOCK_AIR)
									{
										currCubeSides |= 0x1 << i;
									}
								}
								partCube(1.0f, ~currCubeSides, verts, inds, vertsOffset, indsOffset, glm::vec3(currx, curry, currz), BLOCK_ARRAY[blockId].getUpperColor(), BLOCK_ARRAY[blockId].getLowerColor());
							}
						}
					}
				}
				getChunk(cx, cy, cz).setData(vertsOffset, verts, indsOffset, inds);
			}
		}
	}
}

void TileMap::meshNaive()
{
	for (int cx = 0; cx < chunkw; cx++)
	{
		for (int cy = 0; cy < chunkh; cy++)
		{
			for (int cz = 0; cz < chunkd; cz++)
			{
				VertexData * verts = new VertexData[cubeVerts * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
				IndexData * inds = new IndexData[cubeInds * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
				GLushort offsetInds = 0;
				int counter = 0;

				for (int x = 0; x < CHUNK_SIZE; x++)
				{
					for (int y = 0; y < CHUNK_SIZE; y++)
					{
						for (int z = 0; z < CHUNK_SIZE; z++)
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

BLOCK_ID& TileMap::get(int x, int y, int z)
{
	return blocks[x + y * width + z * width * height];
}

Mesh& TileMap::getChunk(int x, int y, int z)
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

bool TileMap::inside(int x, int y, int z)
{
	return x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth;
}
