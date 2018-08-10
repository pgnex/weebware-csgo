#pragma once
#include "Header.h"

class c_frame_stage_notify
{
public:
	c_base_entity * local;
	void pvs_fix();


};

extern c_frame_stage_notify g_frame_stage_notify;