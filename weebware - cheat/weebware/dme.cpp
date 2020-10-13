#include "Header.h"
#include "shared.h"
#include "dme.h"
#include "hook_funcs.h"
#include <iostream>

c_draw_model_execute g_dme;

void hooks::hook_functions::draw_model_execute(void* thisptr, void* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld) {

	if (!ctx)
		return;

	// if its not a player or arms nothing needed
	const char* model_name = g_weebware.g_model_info->getmodelname(pInfo.pModel);
	if (!(strstr(model_name, "models/player")) && !(strstr(model_name, "arms")))
		return;

	g_dme.player_chams(pInfo);

}

void c_draw_model_execute::player_chams(const modelrenderinfo_t& pInfo) {

	// if model isnt a player no point
	const char* model_name = g_weebware.g_model_info->getmodelname(pInfo.pModel);
	if (!(strstr(model_name, "models/player")))
		return;

	// get local
	c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
	if (!local)
		return;

	c_base_entity* model_ent = g_weebware.g_entlist->getcliententity(pInfo.entity_index);
	if (!model_ent || !model_ent->is_valid_player())
		return;

	std::cout << model_name << std::endl;
}


void c_draw_model_execute::hand_chams(const modelrenderinfo_t& pInfo) {

}