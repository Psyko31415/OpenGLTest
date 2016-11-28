#pragma once
#include <cmath>
#include <iostream>

namespace noise
{
	float genNoisei(int x);
	float genNoise(float x, float y);
	float lerp(float a0, float a1, float w);
	float coserp(float a0, float a1, float w);
	float smoothNoise(float x, float y);
	float interpolatedNoise(float x, float y);
	float perlinNoise(float x, float y);
}