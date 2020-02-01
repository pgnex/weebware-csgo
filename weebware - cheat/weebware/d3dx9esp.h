#pragma once
#include "Header.h"

class c_d3dxesp
{
public:

	enum fonts {
		tahoma = 0,
		tahoma_large
	};

	void d9esp_main(IDirect3DDevice9* pDevice);
	void d3dx_reset();

private:
	c_base_entity* local;

	void water_mark();
	void draw_inaccuracy_circle();
	void draw_sniper_crosshair();
	void recoil_crosshair();
	void bomb_timer(c_base_entity* ent);

	int spec_count = 0;
};

extern c_d3dxesp g_d3dxesp;