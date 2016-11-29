#include "utils.h"

typedef struct
{
	int x, y, z;
	bool isUpperColor;
} CubeVertsLocation;

typedef struct
{
	GLushort t1i1, t1i2, t1i3, t2i1, t2i2, t2i3;
} CubeIndsLocation;

CubeVertsLocation cubeVertLocations[] = {
	{ -1,  1,  1, true },
	{  1,  1,  1, true },

	{ -1,  1, -1, true },
	{  1,  1, -1, true },

	{ -1, -1,  1, false },
	{  1, -1,  1, false },

	{ -1, -1, -1, false },
	{  1, -1, -1, false }
};

unsigned char vertsNeededForSide[] = {
	0b00001111,
	0b00110011,
	0b10101010,
	0b11001100,
	0b01010101,
	0b11110000,
};

CubeIndsLocation CubeIndsLocations[] = {
	{ 0, 1, 2, 1, 3, 2 },
	{ 0, 4, 5, 0, 5, 1 },
	{ 1, 5, 3, 5, 7, 3 },
	{ 2, 3, 6, 3, 7, 6 },
	{ 0, 2, 4, 2, 6, 4 },
	{ 5, 4, 6, 7, 5, 6 },
};

void binOut(unsigned char c)
{
	for (int i = 7; i > -1; i--)
	{
		std::cout << (bool)(c & (0x1 << i));
	}
	std::cout << " (" << (int)c << ")" << std::endl;
}

void partCube(float width, unsigned char sides, VertexData * verts, IndexData * inds, int& vertOffset, int& indsOffset, glm::vec3 pos, Float3 upperColor, Float3 lowerColor)
{
	float halfWidth = width / 2;
	unsigned char usedVerts = 0;
	GLushort usedVertsIndex[8] = { 0 };
	int addedVerts = 0, addedInds = 0;
	
	for (int i = 0; i < cubeSides; i++)
	{
		if (sides & (0x1 << i))
		{
			usedVerts |= vertsNeededForSide[i];
		}
	}

	for (int i = 0; i < cubeVerts; i++)
	{
		if (usedVerts & (0x1 << i))
		{
			GLushort index = vertOffset + addedVerts;
			verts[index] = { { cubeVertLocations[i].x * halfWidth + pos.x, cubeVertLocations[i].y * halfWidth + pos.y, cubeVertLocations[i].z * halfWidth + pos.z }, cubeVertLocations[i].isUpperColor ? upperColor : lowerColor };
			usedVertsIndex[i] = index;
			addedVerts++;
		}
	}

	for (int i = 0; i < cubeSides; i++)
	{
		if (sides & (0x1 << i))
		{
			int index = indsOffset + addedInds;
			inds[index++] = { usedVertsIndex[CubeIndsLocations[i].t1i1], usedVertsIndex[CubeIndsLocations[i].t1i2], usedVertsIndex[CubeIndsLocations[i].t1i3] };
			inds[index++] = { usedVertsIndex[CubeIndsLocations[i].t2i1], usedVertsIndex[CubeIndsLocations[i].t2i2], usedVertsIndex[CubeIndsLocations[i].t2i3] };
			addedInds += 2;
		}
	}

	indsOffset += addedInds;
	vertOffset += addedVerts;
}

void cube(float width, Float3 color, Float3 color2, VertexData * verts, IndexData * inds, glm::vec3 offsetVerts, GLushort offsetInds)
{
	float halfWidth = width / 2.0f;
	int vi = 0;
	verts[vi++] = { { -halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z }, color  };
	verts[vi++] = { {  halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z }, color  };

	verts[vi++] = { { -halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z }, color  };
	verts[vi++] = { {  halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z }, color  };

	verts[vi++] = { { -halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z }, color2 };
	verts[vi++] = { {  halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z }, color2 };

	verts[vi++] = { { -halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z }, color2 };
	verts[vi++] = { {  halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z }, color2 };

	GLushort i0 = offsetInds + 0;
	GLushort i1 = offsetInds + 1;
	GLushort i2 = offsetInds + 2;
	GLushort i3 = offsetInds + 3;
	GLushort i4 = offsetInds + 4;
	GLushort i5 = offsetInds + 5;
	GLushort i6 = offsetInds + 6;
	GLushort i7 = offsetInds + 7;

	int ii = 0;
	inds[ii++] = { i0, i1, i2 }; // top
	inds[ii++] = { i1, i3, i2 };

	inds[ii++] = { i0, i4, i5 }; // front
	inds[ii++] = { i0, i5, i1 };

	inds[ii++] = { i1, i5, i3 }; // left
	inds[ii++] = { i5, i7, i3 };

	inds[ii++] = { i2, i3, i6 }; // back
	inds[ii++] = { i3, i7, i6 };

	inds[ii++] = { i0, i2, i4 }; // right
	inds[ii++] = { i2, i6, i4 };

	inds[ii++] = { i5, i4, i6 }; // bottom
	inds[ii++] = { i7, i5, i6 };

	
}

void cube(float width, Float3 color, Float3 color2, std::vector<VertexData>& verts, std::vector<IndexData>& inds, glm::vec3 offsetVerts)
{
	float halfWidth = width / 2.0f;
	GLushort offsetInds = (GLushort)verts.size();

	verts.push_back({ { -halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z }, color });
	verts.push_back({ {  halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z }, color });
	verts.push_back({ { -halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z }, color });
	verts.push_back({ {  halfWidth + offsetVerts.x,  halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z }, color });
	verts.push_back({ { -halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z }, color2 });
	verts.push_back({ {  halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y,  halfWidth + offsetVerts.z }, color2 });
	verts.push_back({ { -halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z }, color2 });
	verts.push_back({ {  halfWidth + offsetVerts.x, -halfWidth + offsetVerts.y, -halfWidth + offsetVerts.z }, color2 });

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

float randf()
{
	return rand() / ((float)RAND_MAX);
}

int randi(int min, int max)
{
	return min + rand() % (max - min);
}
