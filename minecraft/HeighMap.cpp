#include "HeighMap.h"

#define BASE_H_SEARCH_SIZE 2
#define BASE_H_SEARCH_BOX_SIZE BASE_H_SEARCH_SIZE * 2 + 1

int basehPrioMap[BASE_H_SEARCH_BOX_SIZE][BASE_H_SEARCH_BOX_SIZE] = {
	{ 1,2,2,2,1 },
	{ 2,3,3,3,2 },
	{ 2,3,5,3,2 },
	{ 2,3,3,3,2 },
	{ 1,2,2,2,1 },
};

HeightMap::HeightMap() : width(0), depth(0), heights(nullptr) {}

void HeightMap::generate(HeightType minHeight, HeightType maxHeight, uint minStep, uint maxStep, uint _width, uint _depth)
{
	width = _width;
	depth = _depth;
	heights = new HeightType[width * depth];
	smoothHeights = new HeightType[width * depth];
	memset(heights, 0, width * depth);
	memset(smoothHeights, 0, width * depth);

	getNormal(0, 0) = (minHeight + maxHeight) / 2;
	for (uint x = 0; x < width; x++)
	{
		for (uint z = 0; z < depth; z++)
		{
			uint filledNeighbors = 0, neighborsTotal = 0;
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					int nextx = x + i, nextz = z + j;
					if (inside(nextx, nextz))
					{
						filledNeighbors += getNormal(nextx, nextz) ? 1 : 0;
						neighborsTotal += getNormal(nextx, nextz);
					}
				}
			}
			if (!getNormal(x, z))
			{
				HeightType tmpHeight = (HeightType)(neighborsTotal / filledNeighbors) + randi(-minStep, maxStep);
				getNormal(x, z) = tmpHeight < minHeight ? minHeight : (tmpHeight > maxHeight ? maxHeight : tmpHeight);
			}
		}
	}
	noise(minHeight, maxHeight);
	noise(minHeight, maxHeight);
	for (uint x = 0; x < width; x++)
	{
		for (uint z = 0; z < depth; z++)
		{
			HeightType basehmh = getNormal(x, z);
			uint baseh = basehmh * HEIGHT_MAP_TILE_RATIO;
			int basehTot = 0, basehCount = 0;

			/*
			for (int i = -BASE_H_SEARCH_SIZE; i <= BASE_H_SEARCH_SIZE; i++)
			{
				for (int j = -BASE_H_SEARCH_SIZE; j <= BASE_H_SEARCH_SIZE; j++)
				{
					int nexthmx = x + i, nexthmz = z + j;
					if (inside(nexthmx, nexthmz))
					{

						basehCount += basehPrioMap[i + BASE_H_SEARCH_SIZE][j + BASE_H_SEARCH_SIZE];
						basehTot += basehPrioMap[i + BASE_H_SEARCH_SIZE][j + BASE_H_SEARCH_SIZE] * (getNormal(nexthmx, nexthmz) - basehmh) * HEIGHT_MAP_TILE_RATIO;
					}
				}
			}
			*/
			basehCount = basehCount == 0 ? 1 : basehCount;
			get(x, z) = baseh + basehTot / basehCount;
		}
	}
}

void HeightMap::noise(uint minHeight, uint maxHeight)
{
	for (uint x = 0; x < width; x++)
	{
		for (uint z = 0; z < depth; z++)
		{
			int tot = 0, count = 0;
			for (int i = -1; i < 2; i++)
			{
				for (int j = -1; j < 2; j++)
				{
					int nextx = x + i, nextz = z + j;
					if (inside(nextx, nextz))
					{
						count++;
						tot += get(nextx, nextz);
					}
				}
			}
			HeightType tmpHeight = tot / count + (randi(0, 2) ? 1 : -1);
			getNormal(x, z) = tmpHeight < minHeight ? minHeight : (tmpHeight > maxHeight ? maxHeight : tmpHeight);
		}
	}
}

bool HeightMap::inside(int x, int z)
{
	return 0 <= x && x < width && 0 <= z && z < depth;
}

void HeightMap::mesh(std::vector<VertexData>& verts, std::vector<IndexData>& inds, float size)
{
	for (uint x = 0; x < width; x++)
	{
		for (uint z = 0; z < depth; z++)
		{
			cube(size, {randf(),randf(),randf()}, { randf(),randf(),randf() }, verts, inds, glm::vec3(size * x, size * get(x, z), size * z));
		}
	}
}

void HeightMap::print()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < depth; j++)
		{
			std::cout << (int)get(i, j) << " ";
		}
		std::cout << std::endl;
	}
}

HeightMap::~HeightMap()
{
	delete[] heights;
	delete[] smoothHeights;
}

HeightType& HeightMap::getNormal(uint x, uint z)
{
	return heights[x + z * width];
}

HeightType& HeightMap::get(uint x, uint z)
{
	return smoothHeights[x + z * width];
}

uint HeightMap::getw()
{
	return width;
}
uint HeightMap::getd()
{
	return width;
}