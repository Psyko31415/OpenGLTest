#include "RaceGen.h"


glm::vec3 rotateAround(glm::vec3 original, glm::vec3 around, float degrees)
{
	return glm::vec3(glm::normalize(glm::rotate(glm::mat4(1.0f), glm::radians(degrees), around) * glm::vec4(original, 0.0f)));
}

void getArgs(std::string data, float & argv1, float & argv2)
{
	size_t pos = data.find(':');
	char *dummyptr;
	if (pos == std::string::npos || pos >= data.size() - 1)
	{
		argv1 = strtof(data.c_str(), &dummyptr);
		argv2 = 0.0f;
	}
	else
	{
		argv1 = strtof(data.substr(0, pos).c_str(), &dummyptr);
		argv2 = strtof(data.substr(pos + 1).c_str(), &dummyptr);
	}
}

Sprite * raceMap2(const char * filePath, float grayScale, GLuint program)
{
	std::ifstream f(filePath);
	std::vector<VertexData> vertices;
	std::vector<IndexData> indices;

	float width = 2.0f;

	if (f.is_open())
	{
		glm::vec3 mapPos(0.0f, 0.0f, 0.0f);
		glm::vec3 mapDir(1.0f, 0.0f, 0.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		
		float dir = 0;
		std::string command;

		while (std::getline(f, command, ';'))
		{
			if (command.size() < 2)
			{
				std::cout << "Command " << command << " is too short" << std::endl;
				return nullptr;
			}

			char action = command[0];
			float argv1, argv2;
			getArgs(command.substr(1), argv1, argv2);

			glm::vec3 right = glm::cross(mapDir, up);
			GLushort nextIndex = (GLushort)vertices.size();

			if (action == 'F')
			{
				glm::vec3 nextPos = mapPos + mapDir * argv1;
				glm::vec3 tl = nextPos - right * width / 2.0f;
				glm::vec3 tr = nextPos + right * width / 2.0f;
				glm::vec3 bl = mapPos - right * width / 2.0f;
				glm::vec3 br = mapPos + right * width / 2.0f;

				vertices.push_back({ { tl.x, tl.y, tl.z },{ grayScale, grayScale, grayScale } });
				vertices.push_back({ { tr.x, tr.y, tr.z },{ grayScale, grayScale, grayScale } });
				vertices.push_back({ { br.x, br.y, br.z },{ grayScale, grayScale, grayScale } });
				vertices.push_back({ { bl.x, bl.y, bl.z },{ grayScale, grayScale, grayScale } });
				
				GLushort nextIndex1 = nextIndex + 1;
				GLushort nextIndex2 = nextIndex + 2;
				GLushort nextIndex3 = nextIndex + 3;

				indices.push_back({ nextIndex, nextIndex1, nextIndex3 });
				indices.push_back({ nextIndex1, nextIndex2, nextIndex3 });

				mapPos = nextPos;
			}
			else if (action == 'R')
			{
				mapDir = rotateAround(mapDir, up, -argv1);
			}
			else if (action == 'L')
			{
				mapDir = rotateAround(mapDir, up, argv1);
			}
			else if (action == 'U')
			{
				mapDir = rotateAround(mapDir, right, argv1);
				up = rotateAround(up, right, argv1);
			}
			else if (action == 'D')
			{
				mapDir = rotateAround(mapDir, right, -argv1);
				up = rotateAround(up, right, -argv1);
			}
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