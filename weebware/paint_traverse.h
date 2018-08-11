#pragma once
#include "Header.h"

class c_paint_traverse
{
public:
	void draw_water_mark();

	void draw_string(unsigned long font, int x, int y, c_color color, DWORD alignment, const char* msg, ...); // 1 is right, 0 is normal

private:

};

extern c_paint_traverse g_paint_traverse;