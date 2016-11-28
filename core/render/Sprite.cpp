#include "Sprite.h"

void Sprite::render(glm::mat4 vp, GLuint program)
{
	mesh->render(vp * translateMat * scaleMat * rotateMat, program);
}

void Sprite::translate(float x, float y, float z)
{
	translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Sprite::scale(float x, float y, float z)
{
	scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Sprite::scale(float f)
{
	scale(f, f, f);
}

void Sprite::rotate(float angle, float x, float y, float z)
{
	rotateMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(x, y, z));
}

void Sprite::cleanup()
{
	mesh->cleanup();
	delete mesh;
}