#pragma once
#include <fstream>
#include <glm/glm.hpp>

class Vector3;

class SerializeUtils
{
public:
	static void WriteVec3(std::ofstream& file, const glm::vec3& vec);
	static  glm::vec3  ReadVec3(std::ifstream& file);

	static  void WriteString(std::ofstream& file, const std::string& str);
	static  std::string ReadString(std::ifstream& file);
};