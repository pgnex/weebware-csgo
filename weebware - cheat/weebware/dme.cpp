#include "Header.h"
#include "shared.h"
#include "dme.h"
#include "hook_funcs.h"
#include <iostream>

c_draw_model_execute g_draw_model_execute;

void hook_functions::draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
	if (!ctx)
		return;

	g_draw_model_execute.no_hands(pInfo);
	g_draw_model_execute.no_smoke(pInfo);

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