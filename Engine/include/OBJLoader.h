#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>


class OBJLoader
{
public:
	static bool Loader(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<unsigned int>& indices, std::vector<glm::vec3>& normals);

private:
};