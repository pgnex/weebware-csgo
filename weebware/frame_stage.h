#pragma once
#include "Header.h"


class c_frame_stage_notify
{
public:
	c_base_entity * local;
	void pvs_fix();

	// Define size of the skinwheel to the total amount of weapons
	override_skin_style skin_wheel[];
};

struct override_skin_style
{
	int weapon = 0;
	int paint_kit = 0;
	float wear = FLT_MIN;
	float seed = 69;
};

extern c_frame_stage_notify g_frame_stage_notify;