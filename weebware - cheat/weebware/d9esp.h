#pragma once
#include "Header.h"
#include "drawing.h"
#include "events.h"

class c_d9esp
{
public:
	void d9esp_main(IDirect3DDevice9* pDevice);

private:
	c_base_entity* local;
	c_draw draw;
	FeatureFuncs g_event_features;

	void water_mark();
	void draw_inaccuracy_circle();
	void draw_fov_circle();

	int spec_count = 0;
};

extern c_d9esp g_d9esp;