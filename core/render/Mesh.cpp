#include "Mesh.h"

Mesh::Mesh() : Mesh(0, (VertexData*)0, 0, (IndexData*)0) {}

Mesh::Mesh(int vc, VertexData * verts, int ic, IndexData * indes) : vertexCount(vc), indexCount(ic), vertices(verts), indices(indes)
{
	// std::cout << "vc: " << vc << " ic: " << ic << std::endl;
	// std::cout << "mem: " << (vc * sizeof(VertexData) + ic * sizeof(IndexData)) / 1000000 << " MB" << std::endl;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	sendData();
	bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	unbind();
}

Mesh::~Mesh()
{
	unbind();
}

void Mesh::setData(int vc, VertexData * verts, int ic, IndexData * indes)
{
	delete[] vertices;
	delete[] indices;

	vertices = verts;
	indices = indes;
	vertexCount = vc;
	indexCount = ic;

	sendData();
}

void Mesh::sendData()
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(indices[0]), indices, GL_STATIC_DRAW);
	unbind();
}

void Mesh::render(glm::mat4 mvp, GLuint program)
{
	bind();

	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, indexCount * sizeof(indices[0]) / sizeof(indices[0].i1), GL_UNSIGNED_SHORT, 0);

	unbind();
}

void Mesh::bind()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

VertexData * Mesh::getVertices()
{
	return vertices;
}

IndexData * Mesh::getIndices()
{
	return indices;
}

void Mesh::cleanup()
{
	unbind();
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	delete[] vertices;
	delete[] indices;
}
