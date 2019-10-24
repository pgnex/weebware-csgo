#pragma once
#include "Header.h"

class c_frame_stage_notify
{
public:

	c_base_entity* local;

	void pvs_fix();
	void run_clantag();
	void wireframe_smoke();
	void no_smoke();
	void run_skinchanger();
	void legit_aa_resolver();
	void preserve_killfeed();
	void bullet_tracers();
	void third_person();
};

extern std::vector< ImpactData_t > vis_impact_data;
extern c_frame_stage_notify g_frame_stage_notify;