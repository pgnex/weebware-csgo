#include "Header.h"
#include "shared.h"
#include "scene_end.h"
#include "hook_funcs.h"
#include <iostream>

c_sceneend g_sceneend;

void hook_functions::scene_end(void* thisptr, void* edx) {

	if (g_weebware.g_engine->is_taking_screenshot() && g_weebwarecfg.screenshot_proof) {
		return;
	}

	try {
		if (g_weebwarecfg.visuals_glow_enabled) {
			g_sceneend.glow();
		}

		if (g_weebwarecfg.visuals_chams > 0) {
			g_sceneend.chams();
		}
	}
	catch (...) {

	}
}


void init_key_vals(KeyValues* keyValues, char* name)
{
	static DWORD keyval_addr = 0;

	if (!keyval_addr)keyval_addr = g_weebware.pattern_scan("client_panorama.dll", "68 ?? ?? ?? ?? 8B C8 E8 ?? ?? ?? ?? 89 45 FC EB 07 C7 45 ?? ?? ?? ?? ?? 8B 03 56*") + 7;

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

void load_from_buf(KeyValues* keyValues, char const *resourceName, const char *pBuffer, class IBaseFileSystem* pFileSystem, const char *pPathID, void* pUnknown, void* uu)
{
	static  DWORD dwFunction = 0;

	if (!dwFunction) dwFunction = g_weebware.pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89");

	if (!dwFunction) {
		return;
	}

	__asm
	{
		push uu
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
imaterial* c_sceneend::generate_material(bool ignore, bool lit, bool wire_frame)
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
	load_from_buf(keyValues, name, material, 0, 0, 0, 0);
	imaterial* created_mat = g_weebware.g_mat_sys->create_mat(name, keyValues);
	created_mat->incrementreferencecount();
	return created_mat;
}


imaterial* create_default() {
	std::ofstream("csgo\\materials\\material_textured.vmt") << R"#("VertexLitGeneric"
{
	"$basetexture" "vgui/white_additive"
	"$ignorez"      "0"
	"$envmap"       ""
	"$nofog"        "1"
	"$model"        "1"
	"$nocull"       "0"
	"$selfillum"    "1"
	"$halflambert"  "1"
	"$znearer"      "0"
	"$flat"         "1"
}
)#";

	return g_weebware.g_mat_sys->find_material("material_textured", TEXTURE_GROUP_MODEL);
}


imaterial* c_sceneend::create_glow() {

	std::stringstream s;
	s << "\"$envmaptint\"" << " \"[" << 1 << " " << 1 << " " << 1 << "]\"\n\t";

	std::ofstream glow_cham_texture("csgo/materials/mat_glow_cham.vmt");
	glow_cham_texture.precision(3);
	glow_cham_texture << "\"VertexLitGeneric\" {\n\n\t";
	glow_cham_texture << "\"$additive\" \"0\"\n\t";
	glow_cham_texture << "\"$envmap\" \"models/effects/cube_white\"\n\t";
	glow_cham_texture << s.str();
	glow_cham_texture << "\"$envmapfresnel\" \"1\"\n\t";
	glow_cham_texture << "\"$envmapfresnelminmaxexp\" \"[0 1 2]\"\n\t";
	glow_cham_texture << "\"$alpha\" \"0.8\"\n";
	glow_cham_texture << "}";
	glow_cham_texture.close();

	return g_weebware.g_mat_sys->find_material("mat_glow_cham", TEXTURE_GROUP_MODEL);
}

imaterial* c_sceneend::borrow_mat(custom_mats type)
{
	// Thanks Shigure for these mats u sent me like last year 
	const char* material_list[] = { "", "", "", "debug/debugdrawflat", "models/inventory_items/cologne_prediction/cologne_prediction_glass", "models/inventory_items/trophy_majors/crystal_clear", "models/inventory_items/trophy_majors/gold", "models/inventory_items/trophy_majors/crystal_blue" };

	// TEXTURE_GROUP_MODEL : TEXTURE_GROUP_OTHER

	imaterial* mat = g_weebware.g_mat_sys->find_material(material_list[type], TEXTURE_GROUP_MODEL);
	if (!mat)
		return create_default();

	if (mat->iserrormaterial())
		return create_default();

	return g_weebware.g_mat_sys->find_material(material_list[type], TEXTURE_GROUP_MODEL);
}


void c_sceneend::chams() {

	if (g_weebwarecfg.visuals_chams > custom_mats::max)
		g_weebwarecfg.visuals_chams = 1;

	static bool init = false;

	static imaterial* mat_list[custom_mats::max];

	// Setting up mats
	if (!init) {
		// Grab pre-generated materials
		for (auto i = 1; i < custom_mats::max; i++) {
			mat_list[i] = borrow_mat(static_cast<custom_mats>(i));
		}
		// Make our own materials
		mat_list[custom_mats::plain] = create_default();
		mat_list[custom_mats::glow_cham] = create_glow();
		init = true;
	}

	// get localplayer
	auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	for (int i = 1; i <= g_weebware.g_global_vars->maxclients; i++) {
		auto player = g_weebware.g_entlist->getcliententity(i);

		// make sure ent is a valid player
		if (!player->is_valid_player())
			continue;

		// nullptr check localplayer
		if (!local)
			continue;

		// if player is behind smoke and we dont have xqz on, go next
		if (player->trace_from_smoke(*local->m_vecOrigin()) && !g_weebwarecfg.visuals_chams_xqz) 
			continue;

		// setup our colors here..
		c_color col;

		if (player->trace_from_smoke(*local->m_vecOrigin()) && g_weebwarecfg.visuals_chams_xqz) {
			col = c_color(player->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col_xqz : g_weebwarecfg.visuals_chams_col_xqz);
		}
		else {
			col = c_color(player->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col : g_weebwarecfg.visuals_chams_col);
		}

		float col_blend[4] = { col.r / 255.f, col.g / 255.f, col.b / 255.f, 1.f };

		if (g_weebwarecfg.visuals_chams_xqz) {
			c_color xqz_col = c_color(player->m_iTeamNum() == local->m_iTeamNum() ? g_weebwarecfg.visuals_chams_team_col_xqz : g_weebwarecfg.visuals_chams_col_xqz);
			float xqz_col_blend[4] = { xqz_col.r / 255.f, xqz_col.g / 255.f, xqz_col.b / 255.f, 1.f };
			g_weebware.g_render_view->SetBlend(xqz_col.a / 255.f);
			g_weebware.g_render_view->SetColorModulation(xqz_col_blend);
		}

		//if (g_weebwarecfg.misc_legit_aa_enabled && player == local) {
		//	player->set_angles(Vector(0, g_weebware.fake_angle.y, 0));
		//	g_weebware.g_render_view->SetBlend(0.5f);
		//	player->draw_model(1, 255);
		//	g_weebware.g_render_view->SetBlend(1.f);
		//	player->set_angles(Vector(0, g_weebware.real_angle.y, 0));
		//}

		// skip if team chams is off and entity is teammate
		if ((player->m_iTeamNum() == local->m_iTeamNum()) && !g_weebwarecfg.visuals_chams_render_team)
			continue;

		//if (!mat_list[g_weebwarecfg.visuals_chams])
		//	continue;

		//if (mat_list[g_weebwarecfg.visuals_chams]->iserrormaterial())
		//	continue;

		if (local->m_iTeamNum() == player->m_iTeamNum()) {
			if (g_weebwarecfg.visuals_chams_render_team) {
				if (g_weebwarecfg.visuals_chams_xqz) {
					mat_list[g_weebwarecfg.visuals_chams]->incrementreferencecount();
					mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, true);
					g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
					player->draw_model(1, 255);
					g_weebware.g_model_render->forcedmaterialoverride(nullptr);
				}

				// Set material info.
				g_weebware.g_render_view->SetBlend(col.a / 255.f);
				g_weebware.g_render_view->SetColorModulation(col_blend);

				g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
				player->draw_model(1, 255);
				g_weebware.g_model_render->forcedmaterialoverride(nullptr);
			}
		}
		else {
			if (g_weebwarecfg.visuals_chams_xqz) {
				mat_list[g_weebwarecfg.visuals_chams]->incrementreferencecount();
				mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, true);
				g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
				player->draw_model(1, 255);

				g_weebware.g_model_render->forcedmaterialoverride(nullptr);
			}

			if (g_weebwarecfg.visuals_chams == custom_mats::glow_cham) {

				g_weebware.g_render_view->SetBlend(col.a / 255.f);
				g_weebware.g_render_view->SetColorModulation(col_blend);

				c_color glow_clr = g_weebwarecfg.visuals_chams_glow_col;

				mat_list[g_weebwarecfg.visuals_chams]->incrementreferencecount();
				mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, false);
				g_weebware.g_model_render->forcedmaterialoverride(mat_list[custom_mats::plain]);
				player->draw_model(1, 255);

				bool found = false;
				imaterialvar* pVar = mat_list[g_weebwarecfg.visuals_chams]->FindVar("$envmaptint", &found);
				if (found)
					(*(void(__thiscall**)(int, float, float, float))(*(DWORD*)pVar + 44))((uintptr_t)pVar, glow_clr.r / 255.f, glow_clr.g / 255.f, glow_clr.b / 255.f);

				mat_list[g_weebwarecfg.visuals_chams]->incrementreferencecount();
				mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, false);
				g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
				player->draw_model(1, 255);
				g_weebware.g_model_render->forcedmaterialoverride(nullptr);
			}
			else {
				// Set material info.
				g_weebware.g_render_view->SetBlend(col.a / 255.f);
				g_weebware.g_render_view->SetColorModulation(col_blend);


				mat_list[g_weebwarecfg.visuals_chams]->incrementreferencecount();
				mat_list[g_weebwarecfg.visuals_chams]->setmaterialvarflag(material_var_ignorez, false);
				g_weebware.g_model_render->forcedmaterialoverride(mat_list[g_weebwarecfg.visuals_chams]);
				player->draw_model(1, 255);
				g_weebware.g_model_render->forcedmaterialoverride(nullptr);
			}
		}
	}
}

void c_sceneend::glow() {

	auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
	c_color col;

	for (auto i = 0; i < g_weebware.g_glow_obj_manager->m_GlowObjectDefinitions.Count(); i++)
	{
		auto& glowObject = g_weebware.g_glow_obj_manager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<c_base_entity*>(glowObject.m_pEntity);

		// nullptr check
		if (!entity)
			continue;
		// make sure isnt being used already
		if (glowObject.IsUnused())
			continue;
		// check dormant, not valid player because i want to glow on objects
		if (entity->is_dormant())
			continue;

		// apply based on type of entity
		auto class_id = entity->get_client_class()->m_ClassID;

		if (strstr(entity->get_client_class()->m_networkedname, "CWeapon"))
			class_id = -1;
		else if (class_id == 1 || class_id == weapon_revolver || class_id == 46)
			class_id = -1;

		switch (class_id) {
		case 40:
			if (!g_weebwarecfg.visuals_glow_player) continue;
			if (g_weebwarecfg.visuals_glow_hidden_col) col = is_visible(local, entity) ? g_weebwarecfg.visuals_glow_player_col_visible : g_weebwarecfg.visuals_glow_player_col_hidden;
			else col = g_weebwarecfg.visuals_glow_player_col_visible;
			break;
		case 128:
			if (!g_weebwarecfg.visuals_glow_c4) continue;
			col = g_weebwarecfg.visuals_glow_c4_col;
			break;
		case 36:
			if (!g_weebwarecfg.visuals_glow_chicken) continue;
			col = g_weebwarecfg.visuals_glow_chicken_col;
			break;
		case -1:
			if (!g_weebwarecfg.visuals_glow_weapon)
				continue;

			col = g_weebwarecfg.visuals_glow_weapon_col;
			glowObject.m_flRed = col.r / 255.0f;
			glowObject.m_flGreen = col.g / 255.0f;
			glowObject.m_flBlue = col.b / 255.0f;
			glowObject.m_flAlpha = col.a / 255.0f;
			glowObject.m_bRenderWhenOccluded = true;
			glowObject.m_nGlowStyle = 0;
			glowObject.m_bFullBloomRender = false;
			continue;
		default:
			continue;
		}

		// only apply to enemy, chicken is enemy
		if (entity->m_iTeamNum() == local->m_iTeamNum() && !g_weebwarecfg.visuals_glow_team)
			continue;

		if (entity->m_iTeamNum() == local->m_iTeamNum())
			col = g_weebwarecfg.visuals_glow_team_col;

		glowObject.m_flRed = col.r / 255.0f;
		glowObject.m_flGreen = col.g / 255.0f;
		glowObject.m_flBlue = col.b / 255.0f;
		glowObject.m_flAlpha = col.a / 255.0f;
		glowObject.m_bRenderWhenOccluded = true;
		glowObject.m_nGlowStyle = 0;
		glowObject.m_bFullBloomRender = false;
	}
}