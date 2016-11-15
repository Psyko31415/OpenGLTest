#include "HeighMap.h"

HeightMap::HeightMap(HeightType minHeight, HeightType maxHeight, uint minStep, uint maxStep, uint _width, uint _depth) : width(_width), depth(_depth)
{
	heights = new HeightType[width * depth];
	memset(heights, 0, width * depth);
	get(0, 0) = (minHeight + maxHeight) / 2;
	std::cout << get(0, 0) << std::endl;
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
					if (nextx >= 0 && nextx < width && nextz >= 0 && nextz < depth)
					{
						filledNeighbors += get(nextx, nextz) ? 1 : 0;
						neighborsTotal += get(nextx, nextz);
					}
				}
			}
			if (!get(x, z))
			{
				HeightType tmpHeight = (HeightType)(neighborsTotal / filledNeighbors) + randi(-minStep, maxStep);
				get(x, z) = tmpHeight < minHeight ? minHeight : (tmpHeight > maxHeight ? maxHeight : tmpHeight);
			}
		}
	}
	
}

void HeightMap::mesh(std::vector<VertexData>& verts, std::vector<IndexData>& inds, float size, float greyScale)
{
	for (int x = 0; x < width; x++)
	{
		for (int z = 0; z < depth; z++)
		{
			cube(size, greyScale, verts, inds, glm::vec3(size * x, size * get(x, z), size * z));
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
	delete heights;
}

HeightType& HeightMap::get(uint x, uint z)
{
	return heights[x + z * width];
}