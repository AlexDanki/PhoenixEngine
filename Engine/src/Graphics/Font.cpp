#include "Font.h"
#include "FontLibrary.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include <iostream>
#include <glad/glad.h>

bool Font::Load(const std::string& fontPath)
{
	FT_Face face;
	std::string rootPath = "../../../../Assets/";
	if(FT_New_Face(FontLibrary::GetLibrary(), (rootPath + fontPath).c_str(), 0, &face))
	{
		std::cout << "Erro ao carregar Font ttf \n";
		return false;
	}
	FT_Set_Pixel_Sizes(face, 0, m_fontSize);

	for(unsigned c = 0; c < 128; c++)
	{
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Erro ao carregar charactere: \n" << c << "\n";
			continue;
		}

		GLuint texture;
		
		glGenTextures(1, &texture);
		
		glBindTexture(GL_TEXTURE_2D, texture);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character;

		character.textureID = texture;
		character.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		character.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		character.advance = face->glyph->advance.x;

		//m_characters[c] = character;
		m_characters.emplace(c, character);

	}

	FT_Done_Face(face);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	return true;
}

void Font::SetFontSize(unsigned int size)
{
	m_fontSize = size;
}

const Font::Character& Font::GetCharacter(char c) const
{
	return m_characters.at(c);
}