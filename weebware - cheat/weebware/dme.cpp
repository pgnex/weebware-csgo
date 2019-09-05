#include "Header.h"
#include "shared.h"
#include "scene_end.h"
#include "dme.h"
#include "hook_funcs.h"
#include <iostream>

c_draw_model_execute g_draw_model_execute;
c_sceneend g_sceneendme;

void hook_functions::draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
	if (!ctx)
		return;

	//g_draw_model_execute.no_hands(pInfo);
	//g_draw_model_execute.no_smoke(pInfo);

	//g_draw_model_execute.hand_chams(pInfo);
	// g_draw_model_execute.weapon_chams(pInfo);

	g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
}

bool hands_disabled = false;
void c_draw_model_execute::no_hands(const modelrenderinfo_t& pInfo) {

	if (!g_weebwarecfg.no_hands)
		return;

	const model_t* mdl = pInfo.pModel;

	if (strstr(mdl->szName, "arms")) {
		imaterial* mat = g_weebware.g_mat_sys->find_material("arms", TEXTURE_GROUP_MODEL);
		mat->setmaterialvarflag(materialvarflags_t::material_var_no_draw, true);
		g_weebware.g_model_render->forcedmaterialoverride(mat, overridetype_t::override_normal);
	}
}

static bool smoke_disabled = false;
void c_draw_model_execute::no_smoke(const modelrenderinfo_t& pInfo) {
	const model_t* mdl = pInfo.pModel;

	if (g_weebwarecfg.no_smoke && smoke_disabled) {
		for (auto mat_s : c_draw_model_execute::vistasmoke_mats) {
			imaterial* mat = g_weebware.g_mat_sys->find_material(mat_s, TEXTURE_GROUP_OTHER);
			mat->setmaterialvarflag(materialvarflags_t::material_var_no_draw, true);
		}
		smoke_disabled = false;
	}
	else if (!g_weebwarecfg.no_smoke && !smoke_disabled) {
		for (auto mat_s : c_draw_model_execute::vistasmoke_mats) {
			imaterial* mat = g_weebware.g_mat_sys->find_material(mat_s, TEXTURE_GROUP_OTHER);
			mat->setmaterialvarflag(materialvarflags_t::material_var_no_draw, false);
		}
		smoke_disabled = true;
	}
}

void c_draw_model_execute::hand_chams(const modelrenderinfo_t& pInfo) {

	if (!g_weebwarecfg.hand_chams)
		return;

	static bool init = false;

	static imaterial* mat_list[c_sceneend::custom_mats::max];

	// Setting up mats
	if (!init) {
		// Grab pre-generated materials
		for (auto i = 2; i < c_sceneend::custom_mats::max; i++) {
			mat_list[i] = g_sceneendme.borrow_mat(static_cast<c_sceneend::custom_mats>(i));
		}
		// Make our own materials
		mat_list[c_sceneend::custom_mats::plain] = g_sceneendme.generate_material(0, 1, 0);
		init = true;
	}

	const model_t* mdl = pInfo.pModel;

	bool is_arm = (strstr(mdl->szName, "arms")) != nullptr;
	bool is_sleeve = (strstr(mdl->szName, "sleeve")) != nullptr;

	if (!(is_arm || is_sleeve))
		return;

	imaterial* mat = g_weebware.g_mat_sys->find_material(mdl->szName, TEXTURE_GROUP_MODEL);

	if (!mat)
		return;

	c_color col = g_weebwarecfg.hand_cham_col;
	c_color col_xqz = g_weebwarecfg.hand_cham_col_xqz;

	float blend[4] = { col.r / 255.f, col.g / 255.f, col.b / 255.f, 1.f };
	float blend_xqz[4] = { col_xqz.r / 255.f, col_xqz.g / 255.f, col_xqz.b / 255.f, 1.f };

	if (g_weebwarecfg.hand_chams_xqz) {
		mat_list[c_sceneend::custom_mats::plain]->setmaterialvarflag(material_var_ignorez, true);
		g_weebware.g_model_render->forcedmaterialoverride(mat_list[c_sceneend::custom_mats::plain]);
	}
	else {
		mat_list[c_sceneend::custom_mats::plain]->setmaterialvarflag(material_var_ignorez, false);
		g_weebware.g_model_render->forcedmaterialoverride(mat_list[c_sceneend::custom_mats::plain]);
	}
	g_weebware.g_render_view->SetColorModulation(blend);
}
