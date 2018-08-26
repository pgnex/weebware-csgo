#include "frame_stage.h"
#include "Header.h"
#include "shared.h"

c_frame_stage_notify g_frame_stage_notify;
int convert_index_id(int index);

#if 1
void hook_functions::frame_stage_notify(clientframestage_t curStage)
{
	try {
		g_frame_stage_notify.local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

		if (curStage == clientframestage_t::frame_net_update_postdataupdate_start) {
			g_frame_stage_notify.run_skinchanger();
		}

#if 0
		if (curStage == clientframestage_t::frame_render_start)
		{
			g_frame_stage_notify.pvs_fix();
		}
#endif
	}
	catch (...) {}

	g_hooking.o_fsn(curStage);
	// PLH::FnCast(g_hooking.fsn_tramp, g_hooking.o_fsn)(curStage);
}
#endif

void c_frame_stage_notify::pvs_fix()
{
	if (!local)
		return;

	for (int i = 1; i <= 99; i++)
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

				*weapon->m_nModelIndex() = model_index;

				if (viewmodel_weapon && viewmodel_weapon == weapon) {

					*viewmodel->m_nModelIndex() = model_index;

					auto worldmodel_weapon = reinterpret_cast<c_weaponworldmodel*>(g_weebware.g_entlist->getcliententityfromhandle(viewmodel_weapon->GetWeaponWorldModelHandle()));

					if (worldmodel_weapon)
						*worldmodel_weapon->m_nModelIndex() = model_index + 1;
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
		else {

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

		if (g_weebwarecfg.skinchanger_apply_nxt) {
			(*g_weebware.g_client_state)->force_update();
			g_weebwarecfg.skinchanger_apply_nxt = 0;
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
