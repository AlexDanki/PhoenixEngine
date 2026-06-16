#include "OBJLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool OBJLoader::Loader(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, std::vector<glm::vec3>& normals)
{
	std::string rootPath = "../../../../Assets/" + path;
	std::ifstream file(rootPath);

	if(!file.is_open())
	{
		return false;
	}

	int totalLines = 0;
	std::string line;

	while(std::getline(file, line))
	{

		if(line.rfind("v ", 0) == 0)
		{
			std::stringstream ss(line);
			char v;
			float x;
			float y;
			float z;

			ss >> v >> x >> y >> z;

			vertices.push_back(glm::vec3(x, y, z));
			continue;
		}

		if (line.rfind("f ", 0) == 0)
		{
			std::stringstream ss(line);

			char f;
			std::string v1;
			std::string v2;
			std::string v3;

			ss >> f >> v1 >> v2 >> v3;

			auto ExtractPositionIndex =
				[](const std::string& token)
				{
					size_t slashPos = token.find('/');

					return static_cast<unsigned int>(
						std::stoi(
							token.substr(0, slashPos)
						)
						);
				};

			unsigned int a = ExtractPositionIndex(v1);
			unsigned int b = ExtractPositionIndex(v2);
			unsigned int c = ExtractPositionIndex(v3);

			indices.push_back(a - 1);
			indices.push_back(b - 1);
			indices.push_back(c - 1);
		}

		if(line.rfind("vn ", 0) == 0)
		{
			std::stringstream ss(line);

			char  nr;
			unsigned int x;
			unsigned int y;
			unsigned int z;

			ss >> nr >> x >> y >> z;

			normals.push_back(glm::vec3(x, y, z));

		}
		
		totalLines++;
	}

	std::cout << totalLines << "\n";

	return true;
}