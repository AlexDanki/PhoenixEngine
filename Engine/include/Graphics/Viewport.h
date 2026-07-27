#pragma once

struct Viewport
{
	unsigned int Width = 1280;
	unsigned int Hiehgt = 720;

	float aspect = (float)Width / Hiehgt;
};