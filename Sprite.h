#pragma once
#include "Mesh.h"
#include <glm.hpp>
#include <matrix_transform.hpp>

class Sprite
{
	Mesh* mesh;
	glm::mat4 translateMat, rotateMat, scaleMat;


public:
	Sprite() {}
	Sprite(Mesh* _mesh) : Sprite(_mesh, glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f)) {}
	Sprite(Mesh* _mesh, glm::mat4 _translate, glm::mat4 _rotate, glm::mat4 _scale) : mesh(_mesh), translateMat(_translate), rotateMat(_rotate), scaleMat(_scale) {}

	void render(glm::mat4 vp);
	void translate(float x, float y, float z);
	void Sprite::scale(float x, float y, float z);
	void Sprite::scale(float f);
	void Sprite::rotate(float angle, float x, float y, float z);
	void cleanup();
	Mesh * getMesh();
};

