#pragma once
#include <GL\glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>

typedef struct
{
	GLfloat x, y, z;
} Float3;

typedef struct
{
	Float3 pos;
	Float3 color;
} VertexData;

typedef struct
{
	GLushort i1, i2, i3;
} IndexData;

class Mesh
{
private:
	VertexData * vertices;
	int vertexCount;
	IndexData * indices;
	int indexCount;
	GLuint vbo, ibo, vao, program;
	GLint mvpUniformId;

	void bind();
	void unbind();

public:
	Mesh(int vc, VertexData * verts, int ic, IndexData * IndexData, GLuint _program);
	~Mesh();

	VertexData * getVertices();
	IndexData * getIndices();

	void render(glm::mat4 mvp);
	void cleanup();
};