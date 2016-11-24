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
	HeightType * smoothHeights;

	void noise(uint minHeight, uint maxHeight);
	HeightType& getNormal(uint x, uint z);
public:
	HeightMap();
	~HeightMap();

	void generate(HeightType minHeight, HeightType maxHeight, uint minStep, uint maxStep, uint _width, uint _depth);
	void print();
	void mesh(std::vector<VertexData>& verts, std::vector<IndexData>& inds, float size);
	HeightType& get(uint x, uint z);
	bool inside(int x, int z);

	uint getw();
	uint getd();
};

