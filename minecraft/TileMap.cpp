#include "TileMap.h"
#define BLUR_SIZE 16

TileMap::TileMap(uint _chunkw, HeightType _chunkh, uint _chunkd, float blocksize, GLuint program) : chunkw(_chunkw), chunkh(_chunkh), chunkd(_chunkd), arrsize(chunkd * chunkw)
{
	chunks = new Chunk[chunkw * chunkd];
	hm.generate(1, chunkh / HEIGHT_MAP_TILE_RATIO, 3, 3, chunkw, chunkd);
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
					HeightType baseh = hm.get(chunkx, chunkz);
					int count = 1;
					float dy = 0;
					for (int i = -BLUR_SIZE / 2; i < BLUR_SIZE / 2; i++)
					{
						for (int j = -BLUR_SIZE / 2; j < BLUR_SIZE / 2; j++)
						{
							int nexthmx = (x + i) / HEIGHT_MAP_TILE_RATIO, nexthmz = (z + j) / HEIGHT_MAP_TILE_RATIO;
							if (hm.inside(nexthmx, nexthmz))
							{
								float dist = sqrt(i * i + j * j);
								dist = dist == 0 ? 1 : dist;
								dy += 2 * (hm.get(nexthmx, nexthmz) - baseh) / dist;
								count++;
							}
						}
					}
					uint y = baseh + (int)(dy / count);
					y = y < chunkh ? y : chunkh - 1;
					chunk.get(x, y, z) = BLOCK_GRASS;
					for (int i = 0; i < y; i++)
					{
						if (chunkx == 1)
							chunk.get(x, i, z) = BLOCK_STONE;
						else
							chunk.get(x, i, z) = BLOCK_RED;
					}
				}
				//std::cout << std::endl;
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
	for (uint i = 0; i < arrsize; i++)
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

int TileMap::distToCenterOf(uint x, uint z, uint chunkx, uint chunkz)
{
	int dx = (x - (chunkx * CHUNK_SIZE + CHUNK_SIZE / 2)), dz = (z - (chunkz * CHUNK_SIZE + CHUNK_SIZE / 2));
	return (uint)sqrt(dx * dx + dz * dz);
}