#include "RaceGen.h"

#define printvec(vec) std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl
#define ACTION_HISTORY_LEN 4

glm::vec3 rotate(glm::vec3 original, glm::vec3 around, float degrees)
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

bool invalidAction(char pair1, char pair2, char check1, char check2)
{
	return (check1 == pair1 && check2 == pair2) || (check2 == pair1 && check1 == pair2);
}

bool actionValid(char action, float argv1, float argv2, char * lastActions)
{
	static int lastActionPos = 0;

	if (action == 'F')
	{
		memset(lastActions, 0, ACTION_HISTORY_LEN * sizeof(char));
	}
	else
	{
		for (int i = 0; i < ACTION_HISTORY_LEN; i++)
		{
			if (invalidAction('R', 'L', action, lastActions[i]) || invalidAction('U', 'D', action, lastActions[i]))
			{
				printf("Error: Action %c not allowed with %c before a F-action\n", action, lastActions[i]);
				return false;
			}
			if (action == lastActions[i])
			{
				printf("Error: Action %c appears twice before a F-action\n", action);
				return false;
			}
		}
		lastActions[lastActionPos++] = action;
	}
	if ((action == 'R' || action == 'L') && argv1 > 90)
	{
		printf("Error: Turns over 90 degrees are not allowed");
		return false;
	}
	return true;
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
		glm::vec3 lastTurnMapDir = mapDir;

		std::string command;

		bool turned = false;
		GLushort lastTLIndex = 0, lastTRIndex = 1;
		char lastActions[ACTION_HISTORY_LEN] = {0};

		while (std::getline(f, command, ';'))
		{
			if (command.size() < 2)
			{
				std::cout << "Error: Command " << command << " is too short" << std::endl;
				return nullptr;
			}

			char action = command[0];
			float argv1, argv2;
			getArgs(command.substr(1), argv1, argv2);

			if (!actionValid(action, argv1, argv2, lastActions))
			{
				return nullptr;
			}

			glm::vec3 right = glm::cross(mapDir, up);

			if (action == 'F')
			{
				GLushort nextIndex = (GLushort)vertices.size();

				if (turned)
				{
					mapPos += (lastTurnMapDir + mapDir) * (width / 2.0f);
				}

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

				if (turned)
				{
					indices.push_back({ nextIndex3, nextIndex2, lastTRIndex });
					indices.push_back({ nextIndex3, lastTRIndex, lastTLIndex });
				}

				turned = false;
				mapPos = nextPos;
				lastTLIndex = nextIndex;
				lastTRIndex = nextIndex1;
			}
			else if (action == 'R')
			{
				lastTurnMapDir = mapDir;
				mapDir = rotate(mapDir, up, -argv1);
				turned = true;
			}
			else if (action == 'L')
			{
				lastTurnMapDir = mapDir;
				mapDir = rotate(mapDir, up, argv1);
				turned = true;
			}
			else if (action == 'U')
			{
				mapDir = rotate(mapDir, right, argv1);
				up = rotate(up, right, argv1);
			}
			else if (action == 'D')
			{
				mapDir = rotate(mapDir, right, -argv1);
				up = rotate(up, right, -argv1);
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