#include "Header.h"
#include "shared.h"
#include "dme.h"
#include "hook_funcs.h"
#include <iostream>

c_dme g_dme;

void hook_functions::draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
	if (!ctx)
		return;
	try {
		g_dme.draw_model_execute(thisptr, edx, ctx, state, pInfo, pCustomBoneToWorld);
	}
	catch (...) {}
}

void init_key_vals(KeyValues* keyValues, char* name)
{
	static DWORD keyval_addr = 0;

	if (!keyval_addr)keyval_addr = g_weebware.pattern_scan("client.dll", "68 ?? ?? ?? ?? 8B C8 E8 ?? ?? ?? ?? 89 45 FC EB 07 C7 45 ?? ?? ?? ?? ?? 8B 03 56*") + 7;

	static DWORD dwFunction = 0;

	if (!dwFunction) dwFunction = keyval_addr + *reinterpret_cast<PDWORD_PTR>(keyval_addr + 1) + 5;

	if (!dwFunction) {
		return;
	}
	__asm
	{
		push name
		mov ecx, keyValues
		call dwFunction
	}
}

void load_from_buf(KeyValues* keyValues, char const *resourceName, const char *pBuffer, class IBaseFileSystem* pFileSystem, const char *pPathID, void* pUnknown)
{
	static  DWORD dwFunction = 0;

	if (!dwFunction) dwFunction = g_weebware.pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04");

	if (!dwFunction) {
		return;
	}

	__asm
	{
		push pUnknown
		push pPathID
		push pFileSystem
		push pBuffer
		push resourceName
		mov ecx, keyValues
		call dwFunction
	}
}

// Ayyware
imaterial* generate_material(bool ignore, bool lit, bool wire_frame)
{
	static int created = 0;
	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$flat\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$nofog\" \"0\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};

	char* baseType = (lit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	char name[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (ignore) ? 1 : 0, (wire_frame) ? 1 : 0);
	sprintf_s(name, sizeof(name), "#chams%i.vmt", created);
	++created;
	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	init_key_vals(keyValues, baseType);
	load_from_buf(keyValues, name, material, 0, 0, 0);
	imaterial* created_mat = g_weebware.g_mat_sys->create_mat(name, keyValues);
	created_mat->incrementreferencecount();
	return created_mat;
}

imaterial* c_dme::borrow_mat(custom_mats type)
{
	// Thanks Shigure for these mats u sent me like last year 
	const char* material_list[] = { "", "", "models/player/ct_fbi/ct_fbi_glass", "models/inventory_items/cologne_prediction/cologne_prediction_glass", "models/inventory_items/trophy_majors/crystal_clear", "models/inventory_items/trophy_majors/gold", "models/gibs/glass/glass", "models/inventory_items/trophy_majors/gloss", "vgui/achievements/glow", "dev/glow_rim3d" , "models/inventory_items/wildfire_gold/wildfire_gold_detail" ,"models/inventory_items/trophy_majors/crystal_blue" , "models/inventory_items/trophy_majors/velvet", "models/inventory_items/music_kit/darude_01/mp3_detail" };

	// TEXTURE_GROUP_MODEL : TEXTURE_GROUP_OTHER
	return g_weebware.g_mat_sys->find_material(material_list[type], nullptr);
}

// change all return false to original calls.

void c_dme::draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
//	g_dme.night_mode();

	static bool init = false;

	static imaterial* mat_list[custom_mats::max];

	// Setting up mats

	if (!init) {

		// Grab pre-generated materials
		for (auto i = 2; i < custom_mats::max; i++) {
			mat_list[i] = borrow_mat(static_cast<custom_mats>(i));
		}

		// Make our own materials
		mat_list[custom_mats::plain] = generate_material(0, 1, 0);

		init = true;
	}

	if (g_weebwarecfg.visuals_chams > 0) {

		// Get players only
		auto entity = g_weebware.g_entlist->getcliententity(pInfo.entity_index);

		// Get local
		auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());


		// Filtering out players only
		if (entity && entity->is_valid_player() && local && local->is_valid_player() && strstr(g_weebware.g_model_info->getmodelname((model_t*)pInfo.pModel), "models/player")) {

			c_color col = c_color(entity->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col : g_weebwarecfg.visuals_chams_col);

			if (g_weebwarecfg.visuals_chams_xqz) {

				c_color xqz_col = c_color(entity->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col_xqz : g_weebwarecfg.visuals_chams_col_xqz);

				mat_list[g_weebwarecfg.visuals_chams]->colormodulate(xqz_col.r / 255.f, xqz_col.g / 255.f, xqz_col.b / 255.f);

				mat_list[g_weebwarecfg.visuals_chams]->alphamodulate(xqz_col.a / 255.f);
			}

			if (local->m_iTeamNum() == entity->m_iTeamNum()) {
				if (g_weebwarecfg.visuals_chams_render_team) {

					if (g_weebwarecfg.visuals_chams_xqz) {

						mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, true);

						g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams], overridetype_t::override_normal);

						g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
					}

					// Set material info.
					mat_list[g_weebwarecfg.visuals_chams]->colormodulate(col.r / 255.f, col.g / 255.f, col.b / 255.f);
					mat_list[g_weebwarecfg.visuals_chams]->alphamodulate(col.a / 255.f);

					g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams], overridetype_t::override_normal);
				}
			}
			else {
				if (g_weebwarecfg.visuals_chams_xqz) {

					mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, true);

					g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams], overridetype_t::override_normal);

					g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
				}

				mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, false);


				// Set material info.
				mat_list[g_weebwarecfg.visuals_chams]->colormodulate(col.r / 255.f, col.g / 255.f, col.b / 255.f);
				mat_list[g_weebwarecfg.visuals_chams]->alphamodulate(col.a / 255.f);

				g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams], overridetype_t::override_normal);
			}
			g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
			// PLH::FnCast(g_hooking.dme_tramp, g_hooking.o_dme)(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		}
	}

	g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	// PLH::FnCast(g_hooking.dme_tramp, g_hooking.o_dme)(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

	g_weebware.g_model_render->forcedmaterialoverride(0, overridetype_t::override_normal);

}