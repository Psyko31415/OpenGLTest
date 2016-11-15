#include "TileMap.h"

// private functions
void cube(float width, float greyScale, std::vector<VertexData>& verts, std::vector<IndexData>& inds, glm::vec3 offsetVerts);
// private functions end

TileMap::TileMap(uint _width, uint _height, uint _depth, float size, float greyScale, GLuint program) : width(_width), height(_height), depth(_depth)
{
	tiles = new Tile[width * height * depth];
	memset(tiles, 0, width * height * depth);

	HeightMap hm(1, 10, 1, 2, width / HEIGHT_MAP_TILE_RATIO, depth / HEIGHT_MAP_TILE_RATIO);
	hm.print();
	std::vector<VertexData> vertices;
	std::vector<IndexData> indices;

	hm.mesh(vertices, indices, size * HEIGHT_MAP_TILE_RATIO, greyScale);

	VertexData * verts = new VertexData[vertices.size()];
	IndexData * inds = new IndexData[indices.size()];
	memcpy(verts, &vertices[0], vertices.size() * sizeof(VertexData));
	memcpy(inds, &indices[0], indices.size() * sizeof(IndexData));

	Mesh * mesh = new Mesh(vertices.size(), verts, indices.size(), inds, program);
	sprite = new Sprite(mesh);
	sprite->translate(-32, -3 * HEIGHT_MAP_TILE_RATIO, -32); // tmp for debug
}

TileMap::~TileMap()
{
	delete tiles;
	sprite->cleanup();
}

Tile& TileMap::operator[](TileMapPos pos)
{
	return get(pos);
}

Tile& TileMap::get(TileMapPos pos)
{
	return tiles[pos.x + pos.y * width + pos.z * width * height];
}

uint TileMap::getw()
{
	return width;
}
uint TileMap::geth()
{
	return height;
}
uint TileMap::getd()
{
	return depth;
}
void TileMap::render(glm::mat4 vp)
{
	sprite->render(vp);
}