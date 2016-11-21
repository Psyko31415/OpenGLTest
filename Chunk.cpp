#include "Chunk.h"



Chunk::Chunk() : sprite(nullptr), height(0), blocks(nullptr)
{
}


Chunk::~Chunk()
{
	if (sprite != nullptr)
	{
		sprite->cleanup();
	}
	delete[] blocks;
}

void Chunk::init(HeightType _height)
{
	height = _height;
	blocks = new BLOCK_ID[CHUNK_SIZE * height * CHUNK_SIZE];
	memset(blocks, 0, CHUNK_SIZE * height * CHUNK_SIZE);
}

BLOCK_ID& Chunk::get(int x, int y, int z)
{
	return blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * height];
}

void Chunk::genMesh(float size, GLuint program, glm::vec3 offset)
{
	std::vector<VertexData> vertices;
	std::vector<IndexData> indices;
	vertices.reserve(CHUNK_SIZE * height * CHUNK_SIZE * cubeVerts);
	indices.reserve(CHUNK_SIZE * height * CHUNK_SIZE * cubeInds);

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				BLOCK_ID block = get(x, y, z);
				if (block != BLOCK_AIR)
				{
					cube(size, BLOCK_ARRAY[block].getUpperColor(), BLOCK_ARRAY[block].getLowerColor(), vertices, indices, glm::vec3(z, y, x) * size + offset);
				}
			}
		}
	}

	VertexData * verts = new VertexData[vertices.size()];
	IndexData * inds = new IndexData[indices.size()];
	memcpy(verts, &vertices[0], vertices.size() * sizeof(VertexData));
	memcpy(inds, &indices[0], indices.size() * sizeof(IndexData));

	Mesh * mesh = new Mesh(vertices.size(), verts, indices.size(), inds, program);
	sprite = new Sprite(mesh);
}

void Chunk::render(glm::mat4 vp)
{
	sprite->render(vp);
}

HeightType Chunk::geth()
{
	return height;
}
