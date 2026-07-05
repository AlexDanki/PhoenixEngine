#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

class Font
{
public:
	unsigned int m_fontSize = 48;
	struct Character
	{
		unsigned int textureID;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};

	bool Load(const std::string& fontPath);

	void SetFontSize(unsigned int size);
	const Character& GetCharacter(char c) const;

private:
	std::unordered_map<char, Character> m_characters;
};