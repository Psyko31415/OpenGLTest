#include "utils.h"

float randf()
{
	return rand() / ((float)RAND_MAX);
}

int randi(int min, int max)
{
	return min + rand() % (max - min);
}

void cube(float width, float greyScale, std::vector<VertexData>& verts, std::vector<IndexData>& inds, glm::vec3 offsetVerts)
{
	verts.reserve(verts.size() + cubeVerts);
	inds.reserve(inds.size() + cubeInds);
	float halfWidth = width / 2.0f;
	GLushort offsetInds = (GLushort)verts.size();

	verts.push_back({ { -halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z },{ 1.0f, 0.0f, 0.0f } });
	verts.push_back({ { halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z },{ 0.0f, 1.0f, 0.0f } });
	verts.push_back({ { -halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z },{ 0.0f, 0.0f, 1.0f } });
	verts.push_back({ { halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z },{ 1.0f, 1.0f, 0.0f } });
	verts.push_back({ { -halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z },{ 0.0f, 1.0f, 0.0f } });
	verts.push_back({ { halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z },{ 0.0f, 0.0f, 1.0f } });
	verts.push_back({ { -halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z },{ 1.0f, 0.0f, 0.0f } });
	verts.push_back({ { halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z },{ 1.0f, 0.0f, 1.0f } });

	GLushort i0 = offsetInds + 0;
	GLushort i1 = offsetInds + 1;
	GLushort i2 = offsetInds + 2;
	GLushort i3 = offsetInds + 3;
	GLushort i4 = offsetInds + 4;
	GLushort i5 = offsetInds + 5;
	GLushort i6 = offsetInds + 6;
	GLushort i7 = offsetInds + 7;

	inds.push_back({ i0, i1, i2 });
	inds.push_back({ i1, i3, i2 });

	inds.push_back({ i0, i4, i5 });
	inds.push_back({ i0, i5, i1 });

	inds.push_back({ i1, i5, i3 });
	inds.push_back({ i5, i7, i3 });

	inds.push_back({ i2, i3, i6 });
	inds.push_back({ i3, i7, i6 });

	inds.push_back({ i0, i2, i4 });
	inds.push_back({ i2, i6, i4 });

	inds.push_back({ i5, i4, i6 });
	inds.push_back({ i7, i5, i6 });
}