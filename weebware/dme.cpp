#include "Header.h"
#include "shared.h"
#include "dme.h"
#include "hook_funcs.h"

c_dme g_dme;

void hook_functions::draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
	if (!ctx)
		return;

	g_dme.draw_model_execute(thisptr, edx, ctx, state, pInfo, pCustomBoneToWorld);
}

uintptr_t* generate_texture()
{
	return 0;
}

imaterial* c_dme::create_mat(custom_mats type)
{
	// Thanks Shigure for these mats u sent me like last year 
	const char* material_list[] = { "","models/player/ct_fbi/ct_fbi_glass", "models/inventory_items/cologne_prediction/cologne_prediction_glass", "models/inventory_items/trophy_majors/crystal_clear", "models/inventory_items/trophy_majors/gold", "models/gibs/glass/glass", "models/inventory_items/trophy_majors/gloss"};

	// TEXTURE_GROUP_MODEL : TEXTURE_GROUP_OTHER
	return g_weebware.g_mat_sys->find_material(material_list[type], nullptr);
}

// change all return false to original calls.

void c_dme::draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
	static bool init = false;

	static imaterial* mat_list[custom_mats::max];

	// Setting up mats

	if (!init) {

		for (auto i = 0; i < custom_mats::max; i++) {
			mat_list[i] = create_mat(static_cast<custom_mats>(i));
		}

		init = true;
	}

	if (g_weebwarecfg.vis_cfg.visuals_chams > 0) {

		// Get players only
		auto entity = g_weebware.g_entlist->getcliententity(pInfo.entity_index);

		// Get local
		auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

		// Filtering out players only
		if (entity && entity->is_valid_player() && local && local->is_valid_player() && strstr(g_weebware.g_model_info->getmodelname((model_t*)pInfo.pModel), "models/player")) {

			c_color col = c_color(entity->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.sets_cfg.visuals_chams_team : g_weebwarecfg.sets_cfg.visuals_chams);

			// Set material info.
			mat_list[g_weebwarecfg.vis_cfg.visuals_chams]->colormodulate(col.r / 255.f, col.g / 255.f, col.b / 255.f);
			mat_list[g_weebwarecfg.vis_cfg.visuals_chams]->alphamodulate(col.a / 255.f);
			// mat_list[g_weebwarecfg.vis_cfg.visuals_chams]->setmaterialvarflag(material_var_no_draw, true);

			g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.vis_cfg.visuals_chams], overridetype_t::override_normal);

			g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
	}

	g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	g_weebware.g_model_render->forcedmaterialoverride(0, overridetype_t::override_normal);

}