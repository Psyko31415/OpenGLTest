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
	GLuint vbo, ibo, vao;
	GLint mvpUniformId;

	void bind();
	void unbind();

	void sendData();

public:
	Mesh();
	Mesh(int vc, VertexData * verts, int ic, IndexData * IndexData);
	~Mesh();

	VertexData * getVertices();
	IndexData * getIndices();

	void render(glm::mat4 mvp, GLuint program);
	void cleanup();

	void setData(int vc, VertexData * verts, int ic, IndexData * indes);



};