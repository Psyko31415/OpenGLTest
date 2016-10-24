#include "ShapeGen.h"

#define pi 3.14159265359f
#define printvec(vec) std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl

glm::vec3 rotateAround(float degrees, glm::vec3 around, glm::vec3 original)
{
	return glm::vec3(glm::normalize(glm::rotate(glm::mat4(1.0f), glm::radians(degrees), around) * glm::vec4(original, 0.0f)));
}

bool actionMatches(char action, char test)
{
	return action == test || action + 32 == test;
}

void getArgs(std::string data, float & number, float & argv2)
{
	size_t pos = data.find(':');
	char *dummyptr;
	if (pos == std::string::npos || pos >= data.size() - 1)
	{
		number = strtof(data.c_str(), &dummyptr);
		argv2 = 0.0f;
	}
	else
	{
		number = strtof(data.substr(0, pos).c_str(), &dummyptr);
		argv2 = strtof(data.substr(pos + 1).c_str(), &dummyptr);
	}
}

glm::vec3 turn(float number, float argv2, glm::vec3 innerDir, glm::vec3 outerDir, glm::vec3 up, glm::vec3 & mapPos, glm::vec3 & mapDir, std::vector<VertexData> & vertices, std::vector<IndexData> & indices, GLushort nextIndex, float grayScale, float width, float isGoingUp)
{
	float radius = argv2 > 1.0f ? argv2 : 1.0f;
	float magicNumber = (32 / isGoingUp - 0.003f * isGoingUp) * (2.5f * width + 18.5) / radius * (isGoingUp > 0 ? 1.0f : -1.0f);	
	glm::vec3 innerCorner = mapPos + innerDir * width / 2.0f;
	glm::vec3 outerCorner = mapPos + outerDir * width / 2.0f;
	glm::vec3 offset = outerCorner + innerDir * radius;

	vertices.push_back({ { outerCorner.x, outerCorner.y, outerCorner.z },{ grayScale, grayScale, grayScale } });
	vertices.push_back({ { innerCorner.x , innerCorner.y, innerCorner.z },{ grayScale, grayScale, grayScale } });

	glm::vec3 outerVert, innerVert;

	int sections = ceil(abs(number) * sqrt(radius) / (15.0f));

	for (int i = 0; i < sections; i++)
	{
		float angle = number / (float)(sections) * (i + 1);

		glm::vec3 unitRotation = rotateAround(angle, up, outerDir);

		outerVert = unitRotation * radius + offset;
		innerVert = unitRotation * (radius - width) + offset;
		outerVert.y += (i + 1) / magicNumber;
		innerVert.y += (i + 1) / magicNumber;

		vertices.push_back({ { outerVert.x, outerVert.y, outerVert.z },{ grayScale, grayScale, grayScale } });
		vertices.push_back({ { innerVert.x, innerVert.y, innerVert.z },{ grayScale, grayScale, grayScale } });

		GLushort outeri = nextIndex + (i + 1) * 2;
		GLushort inneri = nextIndex + (i + 1) * 2 + 1;
		GLushort lastouteri = nextIndex + i * 2;
		GLushort lastinneri = nextIndex + i * 2 + 1;

		indices.push_back({ outeri, inneri, lastinneri });
		indices.push_back({ lastinneri, lastouteri, outeri });
	}
	mapPos = (innerVert + outerVert) / 2.0f;
	mapDir = rotateAround(number, up, mapDir);
	return up;
}

glm::vec3 rotateDown(float number, float argv2, glm::vec3 & up, glm::vec3 left, glm::vec3 right, glm::vec3 & mapPos, glm::vec3 & mapDir, std::vector<VertexData> & vertices, std::vector<IndexData> & indices, GLushort nextIndex, float grayScale, float width)
{
	float radius = argv2 > 1.0f ? argv2 : 1.0f;
	glm::vec3 leftCorner = mapPos + left * width / 2.0f;
	glm::vec3 rightCorner = mapPos + right * width / 2.0f;

	vertices.push_back({ { leftCorner.x , leftCorner.y, leftCorner.z },{ grayScale, grayScale, grayScale } });
	vertices.push_back({ { rightCorner.x, rightCorner.y, rightCorner.z },{ grayScale, grayScale, grayScale } });

	glm::vec3 leftVert, rightVert;

	int sections = ceil(abs(number) * sqrt(radius) / (15.0f));

	for (int i = 0; i < sections; i++)
	{
		glm::vec3 rotation = rotateAround(number / (float)(sections) * (i + 1), left, up) * radius;

		leftVert = rotation + radius * -up + leftCorner;
		rightVert = rotation + radius * -up + rightCorner;

		vertices.push_back({ { leftVert.x, leftVert.y, leftVert.z },{ grayScale, grayScale, grayScale } });
		vertices.push_back({ { rightVert.x, rightVert.y, rightVert.z },{ grayScale, grayScale, grayScale } });

		GLushort outeri = nextIndex + (i + 1) * 2;
		GLushort inneri = nextIndex + (i + 1) * 2 + 1;
		GLushort lastouteri = nextIndex + i * 2;
		GLushort lastinneri = nextIndex + i * 2 + 1;

		indices.push_back({ outeri, inneri, lastinneri });
		indices.push_back({ lastinneri, lastouteri, outeri });
	}
	mapPos = (rightVert + leftVert) / 2.0f;

	mapDir = rotateAround(number, left, mapDir);
	return glm::normalize(glm::cross(right, mapDir));
}

Sprite * ShapeGen::raceMap(const char * filePath, float grayScale, float width, GLuint program)
{
	std::ifstream f(filePath);
	std::vector<VertexData> vertices;
	std::vector<IndexData> indices;
	if (f.is_open())
	{
		glm::vec3 mapPos(0.0f, 0.0f, 0.0f);
		glm::vec3 mapDir(1.0f, 0.0f, 0.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::vec3 absUp = up;
		char lastAction = '\0';
		float dir = 0;
		bool rotatedRight = false, rotatedLeft = false;
		std::string command;
		while (std::getline(f, command, ';'))
		{
			if (command.size() < 2)
			{
				std::cout << "Command " << command << " is too short" << std::endl;
				return nullptr;
			}

			char action = command[0];

			glm::vec3 left = glm::cross(up, mapDir);
			glm::vec3 right = glm::cross(mapDir, up);
			float number, argv2;

			getArgs(command.substr(1), number, argv2);

			glm::vec3 nextPos = mapPos + mapDir * number;
			GLushort nextIndex = (GLushort)vertices.size();

			if (actionMatches('F', action))
			{
				glm::vec3 tl = nextPos + left * width * (0.5f + 1.0f - cos(glm::radians(dir)));
				glm::vec3 tr = nextPos + right * width * (0.5f + 1.0f - cos(glm::radians(dir)));
				glm::vec3 bl = mapPos + left * width * (0.5f + 1.0f - cos(glm::radians(dir)));
				glm::vec3 br = mapPos + right * width * (0.5f + 1.0f - cos(glm::radians(dir)));

				vertices.push_back({ { tl.x, tl.y, tl.z },{ grayScale, grayScale, grayScale } });
				vertices.push_back({ { tr.x, tr.y, tr.z },{ grayScale, grayScale, grayScale } });

				vertices.push_back({ { bl.x, bl.y, bl.z },{ grayScale, grayScale, grayScale } });
				vertices.push_back({ { br.x, br.y, br.z },{ grayScale, grayScale, grayScale } });

				GLushort nextIndex1 = nextIndex + 1;
				GLushort nextIndex2 = nextIndex + 2;
				GLushort nextIndex3 = nextIndex + 3;

				indices.push_back({ nextIndex, nextIndex1, nextIndex2 });
				indices.push_back({ nextIndex1, nextIndex3, nextIndex2 });

				mapPos = nextPos;
			}
			else if (actionMatches('R', action))
			{
				up = turn(-number, argv2, right, left, absUp, mapPos, mapDir, vertices, indices, nextIndex, grayScale, width, dir);
			}
			else if (actionMatches('L', action))
			{
				up = turn(number, argv2, left, right, absUp, mapPos, mapDir, vertices, indices, nextIndex, grayScale, width, dir);
			}
			else if (actionMatches('U', action))
			{
				dir += number;
				up = rotateDown(-number, argv2, -absUp, left, right, mapPos, mapDir, vertices, indices, nextIndex, grayScale, width);
			}
			else if (actionMatches('D', action))
			{
				dir -= number;
				up = rotateDown(number, argv2, absUp, left, right, mapPos, mapDir, vertices, indices, nextIndex, grayScale, width);
			}
			else if (actionMatches('W', action))
			{
				width = number;
			}
			else
			{
				printf("Unknown action %c", action);
				return nullptr;
			}
			lastAction = action;
		}
		VertexData * verts = &vertices[0];
		IndexData * inds = &indices[0];

		IndexData * heapi = new IndexData[indices.size() * sizeof(IndexData)];
		memcpy(heapi, inds, indices.size() * sizeof(IndexData));

		VertexData * heapv = new VertexData[vertices.size() * sizeof(VertexData)];
		memcpy(heapv, verts, vertices.size() * sizeof(VertexData));

		Mesh * mesh = new Mesh(vertices.size(), heapv, indices.size(), heapi, program);

		return new Sprite(mesh);
	}
	else
	{
		printf("Could not open file %s\n", filePath);
		return nullptr;
	}
}

Sprite * ShapeGen::pyramid(float width, float height, float depth, GLuint program)
{
	float width2 = width / 2, depth2 = depth / 2;

	VertexData vertices[] = {
		{ { -width2, 0.0f, depth2 },
		{ 1.0f, 0.0f, 0.0f } },

		{ { width2, 0.0f, depth2 },
		{ 0.0f, 1.0f, 0.0f } },

		{ { width2, 0.0f, -depth2 },
		{ 0.0f, 0.0f, 1.0f } },

		{ { -width2, 0.0, -depth2 },
		{ 1.0f, 0.0f, 1.0f } },

		{ { 0.0f, height, 0.0f },
		{ 1.0f, 1.0f, 0.0f } },
	};

	IndexData indecies[] = {
		{ 0, 1, 3 },
		{ 1, 2, 3 },
		{ 4, 0, 1 },
		{ 4, 1, 2 },
		{ 4, 2, 3 },
		{ 4, 3, 0 },
	};

	IndexData * heapi = new IndexData[sizeof(indecies) / sizeof(indecies[0])];
	memcpy(heapi, indecies, sizeof(indecies));

	VertexData * heapv = new VertexData[sizeof(vertices) / sizeof(vertices[0])];
	memcpy(heapv, vertices, sizeof(vertices));

	Mesh * mesh = new Mesh(sizeof(vertices) / sizeof(vertices[0]), heapv, sizeof(indecies) / sizeof(indecies[0]), heapi, program);
	return new Sprite(mesh);
}

Sprite * ShapeGen::plane(float width, float depth, float shade, GLuint program)
{
	float width2 = width / 2, depth2 = depth / 2;

	VertexData vertices[] = {
		{ { -width2, 0.0f, depth2 },
		{ shade, shade, shade } },

		{ { width2, 0.0f, depth2 },
		{ shade, shade, shade } },

		{ { width2, 0.0f, -depth2 },
		{ shade, shade, shade } },

		{ { -width2, 0.0f, -depth2 },
		{ shade, shade, shade } },
	};

	IndexData indecies[] = {
		{ 0, 1, 3 },
		{ 1, 2, 3 }
	};

	IndexData * heapi = new IndexData[sizeof(indecies) / sizeof(indecies[0])];
	memcpy(heapi, indecies, sizeof(indecies));

	VertexData * heapv = new VertexData[sizeof(vertices) / sizeof(vertices[0])];
	memcpy(heapv, vertices, sizeof(vertices));

	Mesh * mesh = new Mesh(sizeof(vertices) / sizeof(vertices[0]), heapv, sizeof(indecies) / sizeof(indecies[0]), heapi, program);
	return new Sprite(mesh);
}