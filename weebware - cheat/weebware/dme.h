#pragma once

#include "Header.h"

class c_draw_model_execute {
public:
	void no_hands(const modelrenderinfo_t& pInfo);
	void no_smoke(const modelrenderinfo_t& pInfo);
	void no_flash(const modelrenderinfo_t& pInfo);

	std::vector<const char*> vistasmoke_mats =
	{
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};
}; 
extern c_draw_model_execute g_draw_model_execute;