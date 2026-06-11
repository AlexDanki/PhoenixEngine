#pragma once
#include <string>

class Texture
{
public:
	bool LoadFromFile(const std::string& path);
	unsigned int GetID() const;

private:
	unsigned int m_textureID = 0;
};