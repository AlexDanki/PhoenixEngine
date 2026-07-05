#include "FontLibrary.h"
#include <iostream>

FT_Library FontLibrary::s_library = nullptr;

bool FontLibrary::Init()
{
	if(FT_Init_FreeType(&FontLibrary::s_library))
	{
		std::cout << "Erro ao inicializar freetype " << std::endl;
		return false;
	}

	return true;
}

void FontLibrary::ShutDown()
{
	FT_Done_FreeType(FontLibrary::s_library);
	s_library = nullptr;
}

FT_Library FontLibrary::GetLibrary()
{
	return FontLibrary::s_library;
}