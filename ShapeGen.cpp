#include "ShapeGen.h"

#define pi 3.14159265359f
#define printvec(vec) std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl

Sprite * ShapeGen::pyramid(float width, float height, float depth, GLuint program)
{
	float width2 = width / 2, depth2 = depth / 2;

	VertexData vertices[] = {
		{ { -width2, 0.0f, depth2 },
		{ 1.0f, 0.0f, 0.0f } },

		{ { width2, 0.0f, depth2 },
		{ 0.0f, 1.0f, 0.0f } },

		{ { width2, 0.0f, -depth2 },
		{ 0.0f, 0.0f, 1.0f } },

		{ { -width2, 0.0, -depth2 },
		{ 1.0f, 0.0f, 1.0f } },

		{ { 0.0f, height, 0.0f },
		{ 1.0f, 1.0f, 0.0f } },
	};

	IndexData indecies[] = {
		{ 0, 1, 3 },
		{ 1, 2, 3 },
		{ 4, 0, 1 },
		{ 4, 1, 2 },
		{ 2, 4, 3 },
		{ 3, 4, 0 },
	};

	IndexData * heapi = new IndexData[sizeof(indecies) / sizeof(indecies[0])];
	memcpy(heapi, indecies, sizeof(indecies));

	VertexData * heapv = new VertexData[sizeof(vertices) / sizeof(vertices[0])];
	memcpy(heapv, vertices, sizeof(vertices));

	Mesh * mesh = new Mesh(sizeof(vertices) / sizeof(vertices[0]), heapv, sizeof(indecies) / sizeof(indecies[0]), heapi, program);
	return new Sprite(mesh);
}

Sprite * ShapeGen::plane(float width, float depth, float shade, GLuint program)
{
	float width2 = width / 2, depth2 = depth / 2;

	VertexData vertices[] = {
		{ { -width2, 0.0f, depth2 },
		{ shade, shade, shade } },

		{ { width2, 0.0f, depth2 },
		{ shade, shade, shade } },

		{ { width2, 0.0f, -depth2 },
		{ shade, shade, shade } },

		{ { -width2, 0.0f, -depth2 },
		{ shade, shade, shade } },
	};

	IndexData indecies[] = {
		{ 0, 1, 3 },
		{ 1, 2, 3 }
	};

	IndexData * heapi = new IndexData[sizeof(indecies) / sizeof(indecies[0])];
	memcpy(heapi, indecies, sizeof(indecies));

	VertexData * heapv = new VertexData[sizeof(vertices) / sizeof(vertices[0])];
	memcpy(heapv, vertices, sizeof(vertices));

	Mesh * mesh = new Mesh(sizeof(vertices) / sizeof(vertices[0]), heapv, sizeof(indecies) / sizeof(indecies[0]), heapi, program);
	return new Sprite(mesh);
}