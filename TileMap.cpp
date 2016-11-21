#include "TileMap.h"

TileMap::TileMap(uint _chunkw, HeightType _chunkh, uint _chunkd, float blocksize, GLuint program) : chunkw(_chunkw), chunkh(_chunkh), chunkd(_chunkd), arrsize(chunkd * chunkw)
{
	chunks = new Chunk[chunkw * chunkd];
	hm.generate(1, chunkh / HEIGHT_MAP_TILE_RATIO , 1, 2, chunkw, chunkd);
	hm.print();

	for (uint chunkx = 0; chunkx < chunkw; chunkx++)
	{
		for (uint chunkz = 0; chunkz < chunkd; chunkz++)
		{
			Chunk& chunk = get(chunkx, chunkz);
			chunk.init(chunkh);

			HeightType baseh = hm.get(chunkx, chunkz);

			for (uint x = 0; x < CHUNK_SIZE; x++)
			{
				for (uint z = 0; z < CHUNK_SIZE; z++)
				{
					HeightType y = baseh;
					y = y < chunkh ? y : chunkh - 1;
					chunk.get(x, y, z) = BLOCK_GRASS;
					for (int i = 0; i < y; i++)
					{
						chunk.get(x, i, z) = BLOCK_STONE;
					}
				}
			}
		}
	}
	genMesh(blocksize, program);
}

TileMap::~TileMap()
{
	delete[] chunks;
}

void TileMap::render(glm::mat4 vp)
{
	for (int i = 0; i < arrsize; i++)
	{
		chunks[i].render(vp);
	}
}

void TileMap::genMesh(float size, GLuint program)
{
	for (uint x = 0; x < chunkw; x++)
	{
		for (uint z = 0; z < chunkd; z++)
		{
			get(x, z).genMesh(size, program, glm::vec3(x, 0, z) * (size * CHUNK_SIZE));
		}
	}
}

Chunk& TileMap::get(uint x, uint z)
{
	return chunks[x + z * chunkw];
}