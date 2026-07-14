#include "SerializeUtils.h"
#include <fstream>

void SerializeUtils::WriteVec3(std::ofstream& file, const glm::vec3& vec)
{
	file << vec.x << " "
		<< vec.y << " " 
		<< vec.z << "\n";
}
glm::vec3 SerializeUtils::ReadVec3(std::ifstream& file)
{
	glm::vec3 vec;

	file >> 
		vec.x >> 
		vec.y >> 
		vec.z;

	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha)

	return vec;
}

void SerializeUtils::WriteString(std::ofstream& file, const std::string& str)
{
	file << str << "\n";
}

std::string SerializeUtils::ReadString(std::ifstream& file)
{
	std::string str;
	file >> str;
	return str;
}	