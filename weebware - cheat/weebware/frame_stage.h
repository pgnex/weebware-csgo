#pragma once
#include "Header.h"

class c_frame_stage_notify
{
public:

	c_base_entity* local;

	void pvs_fix();
	void run_clantag();
	bool clantag_done = false;
	void wireframe_smoke();
	void no_smoke();
	void run_skinchanger();
	void legit_aa_resolver();
	void preserve_killfeed();
	void bullet_tracers();
	int convert_index_id(int index);
	void no_vis_recoil(bool restore = false);
};

struct ImpactData_t;
extern std::vector< ImpactData_t > vis_impact_data;
extern c_frame_stage_notify g_frame_stage_notify;