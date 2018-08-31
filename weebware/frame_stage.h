#pragma once
#include "Header.h"

class c_frame_stage_notify
{
public:

	c_base_entity* local;

	void pvs_fix();
	void run_skinchanger();
	void legit_aa_resolver();
};


extern c_frame_stage_notify g_frame_stage_notify;