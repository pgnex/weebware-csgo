#include "frame_stage.h"
#include "Header.h"
#include "shared.h"

c_frame_stage_notify g_frame_stage_notify;

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

		auto weapon_id = weapon->filtered_index();

		if (weapon_id > 99 || weapon_id < 0)
			continue;

		if (weapon_id > 99)
			weapon_id = 99;


		if (weapon_id <= 0)
			weapon_id = 0;


		*weapon->get_item_id_high() = -1;

		if (weapon->is_knife() && weapon_id == 69) {

			auto vm_handle = local->get_viewmodel_handle();

			auto viewmodel = reinterpret_cast<c_viewmodel*>(g_weebware.g_entlist->getcliententityfromhandle(vm_handle));

			if (!viewmodel)
				continue;

			auto viewmodel_weapon = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(viewmodel->get_weapon_handle()));

			auto knife_cfg = g_weebwarecfg.selected_knife;
#pragma region knifechanger
			if (knife_cfg.weapon_index != 0) {

				auto model_index = g_weebware.g_model_info->getmodelindex(knife_cfg.mdl.c_str());

				*weapon->m_nModelIndex() = model_index;

				if (viewmodel_weapon == weapon) {

					*viewmodel->m_nModelIndex() = model_index;

					auto worldmodel_weapon = reinterpret_cast<c_weaponworldmodel*>(g_weebware.g_entlist->getcliententityfromhandle(viewmodel_weapon->GetWeaponWorldModelHandle()));

					if (worldmodel_weapon)
						*worldmodel_weapon->m_nModelIndex() = model_index + 1;
				}

				auto skin_config = g_weebwarecfg.skin_wheel[69];

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
			auto skin_config = g_weebwarecfg.skin_wheel[weapon_id];

			if (skin_config.m_paint_kit != 0)
				*weapon->get_paint_kit() = skin_config.m_paint_kit;

			*weapon->get_fallbackseed() = skin_config.m_seed;

			// Config Clamping.
			if (skin_config.m_wear < FLT_MIN)
				skin_config.m_wear = FLT_MIN;

			*weapon->get_fallbackwear() = skin_config.m_wear;

			//std::string custom_name = skin_config.name;
			//if (custom_name != "")
			//	sprintf(weapon->get_custom_name(), custom_name.c_str());

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

/*
Dump by Kite

csgo/scripts/items/items_game.txt

"522"
{
"name"		"weapon_knife_stiletto"
"prefab"		"melee_unusual"
"item_name"		"#SFUI_WPNHUD_knife_stiletto"
"item_description"		"#CSGO_Item_Desc_knife_stiletto"
"image_inventory"		"econ/weapons/base_weapons/weapon_knife_stiletto"
"icon_default_image"		"materials/icons/inventory_icon_weapon_knife_stiletto.vtf"
"model_player"		"models/weapons/v_knife_stiletto.mdl"
"model_world"		"models/weapons/w_knife_stiletto.mdl"
"model_dropped"		"models/weapons/w_knife_stiletto_dropped.mdl"
}



"519"
{
"name"		"weapon_knife_ursus"
"prefab"		"melee_unusual"
"item_name"		"#SFUI_WPNHUD_knife_ursus"
"item_description"		"#CSGO_Item_Desc_knife_ursus"
"image_inventory"		"econ/weapons/base_weapons/weapon_knife_ursus"
"icon_default_image"		"materials/icons/inventory_icon_weapon_knife_ursus.vtf"
"model_player"		"models/weapons/v_knife_ursus.mdl"
"model_world"		"models/weapons/w_knife_ursus.mdl"
"model_dropped"		"models/weapons/w_knife_ursus_dropped.mdl"


"523"
{
"name"		"weapon_knife_widowmaker"
"prefab"		"melee_unusual"
"item_name"		"#SFUI_WPNHUD_knife_widowmaker"
"item_description"		"#CSGO_Item_Desc_knife_widowmaker"
"image_inventory"		"econ/weapons/base_weapons/weapon_knife_widowmaker"
"icon_default_image"		"materials/icons/inventory_icon_weapon_knife_widowmaker.vtf"
"model_player"		"models/weapons/v_knife_widowmaker.mdl"
"model_world"		"models/weapons/w_knife_widowmaker.mdl"
"model_dropped"		"models/weapons/w_knife_widowmaker_dropped.mdl"


"520"
{
"name"		"weapon_knife_gypsy_jackknife"
"prefab"		"melee_unusual"
"item_name"		"#SFUI_WPNHUD_knife_gypsy_jackknife"
"item_description"		"#CSGO_Item_Desc_knife_gypsy_jackknife"
"image_inventory"		"econ/weapons/base_weapons/weapon_knife_gypsy_jackknife"
"icon_default_image"		"materials/icons/inventory_icon_weapon_knife_gypsy_jackknife.vtf"
"model_player"		"models/weapons/v_knife_gypsy_jackknife.mdl"
"model_world"		"models/weapons/w_knife_gypsy_jackknife.mdl"
"model_dropped"		"models/weapons/w_knife_gypsy_jackknife_dropped.mdl"

*/

