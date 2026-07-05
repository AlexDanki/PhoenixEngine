#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H


class FontLibrary
{
public:
	static bool Init();
	static void ShutDown();
	static FT_Library GetLibrary();

private:
	static FT_Library s_library;
};