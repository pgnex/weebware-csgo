#include "frame_stage.h"
#include "Header.h"
#include "shared.h"
#include "skinchanger.h"
#include "thirdperson.h"

c_frame_stage_notify g_frame_stage_notify;
knife_changer g_knife_changer;
int convert_index_id(int index);

#if 1
void hook_functions::frame_stage_notify(clientframestage_t curStage)
{

	try {
		g_frame_stage_notify.local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());


		if (curStage == clientframestage_t::frame_net_update_postdataupdate_start) {
			g_frame_stage_notify.run_skinchanger();
			g_frame_stage_notify.third_person();
			g_frame_stage_notify.legit_aa_resolver();
		}

		if (curStage == clientframestage_t::frame_render_start && g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		{
			g_frame_stage_notify.run_clantag();
			g_frame_stage_notify.wireframe_smoke();
		}
	}
	catch (...) {}

	g_hooking.o_fsn(curStage);
	// PLH::FnCast(g_hooking.fsn_tramp, g_hooking.o_fsn)(curStage);
}
#endif

std::vector<const char*> vistasmoke_mats =
{
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
};

static bool wireframe_disabled = false;
void c_frame_stage_notify::wireframe_smoke() {

	if (!g_weebwarecfg.wireframe_smoke && wireframe_disabled == true)
		return;

	if (g_weebwarecfg.wireframe_smoke && wireframe_disabled) {
		for (auto mat_s : vistasmoke_mats) {
			imaterial* mat = g_weebware.g_mat_sys->find_material(mat_s, TEXTURE_GROUP_OTHER);
			mat->setmaterialvarflag(materialvarflags_t::material_var_ignorez, false);
			mat->setmaterialvarflag(materialvarflags_t::material_var_wireframe, true);
		}
		wireframe_disabled = false;
	}
	else if (!g_weebwarecfg.wireframe_smoke && !wireframe_disabled) {
		for (auto mat_s : vistasmoke_mats) {
			imaterial* mat = g_weebware.g_mat_sys->find_material(mat_s, TEXTURE_GROUP_OTHER);
			mat->setmaterialvarflag(materialvarflags_t::material_var_ignorez, false);
			mat->setmaterialvarflag(materialvarflags_t::material_var_wireframe, false);
		}
		wireframe_disabled = true;
	}
}


static auto set_clantag = (int(__fastcall*)(const char*, const char*))(g_weebware.pattern_scan("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
bool clantag_done = false;
void c_frame_stage_notify::run_clantag()
{

	if (!g_weebwarecfg.misc_clantag_changer && clantag_done) {
		set_clantag("", "");
		clantag_done = false;
		return;
	}

	if (!g_weebwarecfg.misc_clantag_changer) {
		return;
	}


	const char* stages[] = { u8"\u2800\u2800\u2800\u2800w", u8"\u2800\u2800\u2800w\u2800", u8"\u2800\u2800w\u2800\u2800", u8"\u2800\u2800w\u2800\u2800", u8"\u2800w\u2800\u2800\u2800", u8"\u2800w\u2800\u2800\u2800", u8"w\u2800\u2800\u2800\u2800", u8"w\u2800\u2800\u2800\u2800e", u8"w\u2800\u2800\u2800e\u2800", u8"w\u2800\u2800e\u2800\u2800", u8"w\u2800e\u2800\u2800\u2800", u8"w\u2800e\u2800\u2800\u2800", u8"we\u2800\u2800\u2800\u2800", u8"we\u2800\u2800\u2800\u2800e", u8"we\u2800\u2800\u2800e\u2800", u8"we\u2800\u2800e\u2800\u2800", u8"we\u2800e\u2800\u2800\u2800", u8"wee\u2800\u2800\u2800\u2800", u8"wee\u2800\u2800\u2800b", u8"wee\u2800\u2800b\u2800", u8"wee\u2800b\u2800\u2800", u8"weeb\u2800\u2800\u2800", u8"weeb\u2800\u2800w", u8"weeb\u2800w\u2800", u8"weebw\u2800a\u2800", u8"weebwa\u2800\u2800", u8"weebwar\u2800", u8"weebware", u8"weebware", u8"\u2800\u2800\u2800\u2800\u2800\u2800\u2800", u8"\u2800\u2800\u2800\u2800\u2800\u2800\u2800", u8"weebware", u8"weebware" };

	static int current;
	int serverTime = g_weebware.g_global_vars->interval_per_tick * (float)this->local->get_tick_base() * 2.5;
	int value = serverTime % 33;

	if (value != current) {
		set_clantag(stages[value], stages[value]);
		clantag_done = true;
	}
	current = value;
}

void c_frame_stage_notify::pvs_fix()
{
	if (!local)
		return;

	for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++)
	{
		if (i == local->EntIndex())
			continue;

		c_base_entity* ent = g_weebware.g_entlist->getcliententity(i);

		if (!ent->is_valid_player())
			continue;

		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ent) + 0xA30) = g_weebware.g_global_vars->framecount;
		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ent) + 0xA28) = NULL;
	}
}

bool tp_done = false;
void c_frame_stage_notify::third_person() {
	if (g_weebwarecfg.thirdperson && !tp_done) {
		if (local && local->is_valid_player()) {
			c_convar* tp = g_weebware.g_convars->find_cvar("thirdperson");
			tp->SetValue("1");
			tp_done = true;
		}
	}
	else if (!g_weebwarecfg.thirdperson && tp_done) {
		if (local && local->is_valid_player()) {
			c_convar* fp = g_weebware.g_convars->find_cvar("firstperson");
			fp->SetValue("1");
			tp_done = false;
		}
	}
}

void c_frame_stage_notify::legit_aa_resolver()
{
	if (g_weebwarecfg.misc_legit_aa_resolver)
	{
		if (GetAsyncKeyState(g_weebwarecfg.anti_triggerbot_key))
			return;

		if (!local)
			return;

		if (!local->is_valid_player())
			return;

		for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); ++i)
		{
			auto player = reinterpret_cast<c_base_entity*>(g_weebware.g_entlist->getcliententity(i));

			if (!player)
				continue;

			if (!player->is_valid_player())
				continue;

			if (player->m_iTeamNum() == local->m_iTeamNum())
				continue;

			float delta = fabs(player->m_flLowerBodyYawTarget() - player->m_angEyeAngles().y);

			if (player->m_fFlags() & fl_onground)
			{
				if (player->m_vecVelocity().size() < 0.f) {
					if (delta > 35)
						player->eyeangle_ptr()->y = delta;
				}
				else
				{
					player->eyeangle_ptr()->y = player->m_flLowerBodyYawTarget();
				}
			}
		}
	}
}

void c_frame_stage_notify::run_skinchanger()
{
	if (!g_weebwarecfg.skinchanger_enabled)
		return;

	g_frame_stage_notify.local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!g_frame_stage_notify.local)
		return;

	if (g_frame_stage_notify.local->m_iHealth() <= 0)
		return;

	player_info local_inf;

	if (!g_weebware.g_engine->GetPlayerInfo(g_weebware.g_engine->get_local(), &local_inf))
		return;

	auto weapons = g_frame_stage_notify.local->get_weapons();
	// Skin Changer

	if (!weapons)
		return;

	for (size_t i = 0; weapons[i] != 0xFFFFFFFF; i++)
	{
		if (weapons[i] == 0xFFFFFFFF)
			break;

		auto weapon = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(reinterpret_cast<HANDLE>(weapons[i])));

		if (!weapon)
			continue;

		int weapon_id = convert_index_id(weapon->filtered_index());

		if (weapon_id > 34 || weapon_id < 0)
			continue;

		*weapon->get_item_id_high() = -1;

		if (weapon_id == 0 && weapon->is_knife()) {

			auto vm_handle = local->get_viewmodel_handle();

			if (!vm_handle)
				continue;

			auto viewmodel = reinterpret_cast<c_viewmodel*>(g_weebware.g_entlist->getcliententityfromhandle(vm_handle));

			if (!viewmodel)
				continue;

			auto viewmodel_weapon = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(viewmodel->get_weapon_handle()));


			c_skinchanger::knife_type knife_cfg = g_weebware.g_knife_list[g_weebwarecfg.selected_knife_index[1]];

#pragma region knifechanger
			if (knife_cfg.weapon_index != 0) {

				auto model_index = g_weebware.g_model_info->getmodelindex(knife_cfg.mdl.c_str());

				if (model_index)
				{
					*weapon->m_nModelIndex() = model_index;

					if (viewmodel_weapon && viewmodel_weapon == weapon) {

						*viewmodel->m_nModelIndex() = model_index;

						auto worldmodel_weapon = reinterpret_cast<c_weaponworldmodel*>(g_weebware.g_entlist->getcliententityfromhandle(viewmodel_weapon->GetWeaponWorldModelHandle()));

						if (worldmodel_weapon)
							*worldmodel_weapon->m_nModelIndex() = model_index + 1;
					}

				}

				auto skin_config = g_weebwareskinscfg.skin_wheel[weapon_id];

				if (skin_config.m_paint_kit != 0)
					*weapon->get_paint_kit() = skin_config.m_paint_kit;

				*weapon->get_fallbackseed() = skin_config.m_seed;

				if (skin_config.m_wear < FLT_MIN)
					skin_config.m_wear = FLT_MIN;

				*weapon->get_fallbackwear() = skin_config.m_wear;
				*weapon->get_accountid() = local_inf.xuid_low;
				*weapon->m_iItemDefinitionIndexPtr() = knife_cfg.weapon_index;
				*weapon->get_original_owner_xuidhigh() = 0;
				*weapon->get_original_owner_xuidlow() = 0;

			}
		}
		else if (weapon->is_firearm()) {

#pragma endregion
			auto skin_config = g_weebwareskinscfg.skin_wheel[weapon_id];

			if (skin_config.m_paint_kit != 0)
				*weapon->get_paint_kit() = skin_config.m_paint_kit;

			if (skin_config.m_seed != 0)
				*weapon->get_fallbackseed() = skin_config.m_seed;

			// Config Clamping.
			if (skin_config.m_wear < FLT_MIN)
				skin_config.m_wear = FLT_MIN;

			*weapon->get_fallbackwear() = skin_config.m_wear;


			*weapon->get_accountid() = local_inf.xuid_low;

			*weapon->get_original_owner_xuidhigh() = 0;
			*weapon->get_original_owner_xuidlow() = 0;
		}


	}

}

int convert_index_id(int index)
{
	switch (index) {
	case  69:			return	0;
	case  4:			return	1;
	case  36:			return	2;
	case  1:			return	3;
	case  30:			return	4;
	case  63:			return	5;
	case  64:			return	6;
	case  32:			return	7;
	case  61:			return	8;
	case  2:			return	9;
	case  3:			return	10;
	case  29:			return	11;
	case  25:			return	12;
	case  27:			return	13;
	case  35:			return	14;
	case  17:			return	15;
	case  33:			return	16;
	case  24:			return	17;
	case  26:			return	18;
	case  19:			return	19;
	case  34:			return	20;
	case  23:			return	21;
	case  13:			return	22;
	case  7:			return	23;
	case  39:			return	24;
	case  10:			return	25;
	case  16:			return	26;
	case  60:			return	27;
	case  8:			return	28;
	case  9:			return	29;
	case  38:			return	30;
	case  40:			return	31;
	case  11:			return	32;
	case  14:			return	33;
	case  28:			return	34;
	default: return 0;
	}
}
