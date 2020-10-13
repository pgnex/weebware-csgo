#pragma once

#include "Header.h"

class c_draw_model_execute {
public:
	void player_chams(const modelrenderinfo_t& pInfo);
	void hand_chams(const modelrenderinfo_t& pInfo);

	std::vector<const char*> vistasmoke_mats =
	{
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};
}; 
extern c_draw_model_execute g_draw_model_execute;