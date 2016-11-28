#include "Noise.h"

float noise::genNoisei(int x)
{
	x = (x << 13) ^ x;
	return 1.0f - ((x * (x * x * 15731 + 789221) & 0x7fffffff) / 1073741824.0f);
}

float noise::genNoise(float x, float y)
{
	int n = (int)(x * y * 573);
	return genNoisei(n);
}

float noise::lerp(float a0, float a1, float w)
{
	return a0 * (1 - w) + a1 * w;
}

float noise::coserp(float a0, float a1, float w)
{
	float ft = w * 3.14159265359f;
	float f = (1.0f - (float)cos(ft)) * 0.5f;
	return lerp(a0, a1, f);
}

float noise::smoothNoise(float x, float y)
{
	x = (int)x;
	float corners = (genNoise(x - 1, y - 1) + genNoise(x - 1, y + 1) + genNoise(x + 1, y - 1) + genNoise(x + 1, y + 1)) / 16.0f;
	float sides = (genNoise(x + 1, y) + genNoise(x - 1, y) + genNoise(x, y + 1) + genNoise(x, y - 1)) / 8.0f;
	float center = genNoise(x, y) / 4.0f;

	return corners + sides + center;
}
float noise::interpolatedNoise(float x, float y)
{
	int intX = (int)x;
	float fractionX = x - intX;

	int intY = (int)y;
	float fractionY = y - intY;

	float v1 = smoothNoise(intX, intY);
	float v2 = smoothNoise(intX, intY + 1);
	float v3 = smoothNoise(intX + 1, intY);
	float v4 = smoothNoise(intX + 1, intY + 1);

	float i1 = coserp(v1, v2, fractionX);
	float i2 = coserp(v3, v4, fractionX);

	return coserp(i1, i2, fractionY);
}

float noise::perlinNoise(float x, float y)
{
	float total = 0.0f;

	float p = 0.5f;
	int n = 8;

	for (int i = 0; i < n; i++)
	{
		float freq = (float)pow(2, i) / (1 << n);
		float ampl = (float)pow(p, i + 1);
		total += interpolatedNoise(x * freq, y * freq) * ampl;
	}
	return total;
}
