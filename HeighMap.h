#pragma once
#include <string>
#include <iostream>
#include <vector>

#include "Mesh.h"
#include "utils.h"

class HeightMap
{
private:
	uint width, depth;
	HeightType * heights;

public:
	HeightMap(HeightType minHeight, HeightType maxHeight, uint minStep, uint maxStep, uint _width, uint _depth);
	~HeightMap();

	void print();
	void mesh(std::vector<VertexData>& verts, std::vector<IndexData>& inds, float size, float greyScale);
	HeightType& get(uint x, uint z);
};

