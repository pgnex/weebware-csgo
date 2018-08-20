#include "frame_stage.h"
#include "Header.h"
#include "shared.h"

c_frame_stage_notify g_frame_stage_notify;

#if 1
void __stdcall hook_functions::frame_stage_notify(clientframestage_t curStage)
{
	g_frame_stage_notify.local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (curStage == clientframestage_t::frame_net_update_postdataupdate_start) {
		g_frame_stage_notify.run_skinchanger();
	}

	if (curStage == clientframestage_t::frame_render_start)
	{
		g_frame_stage_notify.pvs_fix();
	}

	g_hooking.o_fsn(curStage);
}
#endif

void c_frame_stage_notify::pvs_fix()
{
	if (!local)
		return;

	for (int i = 1; i <= 99; i++)
	{
		if (i == local->index)
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

	if (!g_frame_stage_notify.local)
		return;

	player_info local_inf;

	g_weebware.g_engine->GetPlayerInfo(g_frame_stage_notify.local->index, &local_inf);

	auto weapons = g_frame_stage_notify.local->get_weapons();
	// Skin Changer

	for (size_t i = 0; weapons[i] != 0xFFFFFFFF; i++)
	{
		auto weapon = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(reinterpret_cast<HANDLE>(weapons[i])));

		if (!weapon)
			continue;

		auto weapon_id = weapon->filtered_index();

		auto vm_handle = local->get_viewmodel_handle();

		auto viewmodel = reinterpret_cast<c_viewmodel*>(g_weebware.g_entlist->getcliententityfromhandle(vm_handle));

		auto viewmodel_weapon = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(viewmodel->get_weapon_handle()));

		if (weapon == viewmodel_weapon && weapon_id == 69) {

			g_weebwarecfg.previous_knife_index = weapon_id;

			auto knife_cfg = g_weebwarecfg.selected_knife;

			if (knife_cfg.weapon_index != 0) {

				auto model_index = g_weebware.g_model_info->getmodelindex(knife_cfg.mdl.c_str());
				*viewmodel->m_nModelIndex() = model_index;

				weapon->set_model_index(model_index);

				*weapon->m_iItemDefinitionIndexPtr() = knife_cfg.weapon_index;

				g_weebwarecfg.next_knife_index = knife_cfg.weapon_index;

				auto worldmodel_weapon = reinterpret_cast<c_weaponworldmodel*>(g_weebware.g_entlist->getcliententityfromhandle(viewmodel_weapon->GetWeaponWorldModelHandle()));

				*worldmodel_weapon->m_nModelIndex() = model_index + 1;

				auto skin_config = g_weebwarecfg.skin_wheel[weapon_id];

				*weapon->get_item_id_high() = -1;

				if (skin_config.m_paint_kit != 0)
					*weapon->get_paint_kit() = skin_config.m_paint_kit;

				*weapon->get_fallbackseed() = skin_config.m_seed;

				// Config Clamping.
				if (skin_config.m_wear < FLT_MIN)
					skin_config.m_wear = FLT_MIN;

				*weapon->get_fallbackwear() = skin_config.m_wear;

				std::string custom_name = skin_config.name;

				if (custom_name != "")
					sprintf(weapon->get_custom_name(), custom_name.c_str());

				*weapon->get_accountid() = local_inf.xuid_low;

				*weapon->get_original_owner_xuidhigh() = 0;
				*weapon->get_original_owner_xuidlow() = 0;

			}

		} 



		if (weapon_id > 100)
			continue;

		auto skin_config = g_weebwarecfg.skin_wheel[weapon_id];

		*weapon->get_item_id_high() = -1;

		if (skin_config.m_paint_kit != 0)
			*weapon->get_paint_kit() = skin_config.m_paint_kit;

		*weapon->get_fallbackseed() = skin_config.m_seed;

		// Config Clamping.
		if (skin_config.m_wear < FLT_MIN)
			skin_config.m_wear = FLT_MIN;

		*weapon->get_fallbackwear() = skin_config.m_wear;

		std::string custom_name = skin_config.name;

		if (custom_name != "")
			sprintf(weapon->get_custom_name(), custom_name.c_str());

		*weapon->get_accountid() = local_inf.xuid_low;

		*weapon->get_original_owner_xuidhigh() = 0;
		*weapon->get_original_owner_xuidlow() = 0;

		if (g_weebware.call_full_update) {

			static auto call_update = reinterpret_cast<void(*)()>(g_weebware.pattern_scan("engine.dll", "A1 ? ? ? ? B9 ? ? ? ? 56 FF 50 14 8B 34 85 ? ? ? ?"));

			call_update();

			g_weebware.call_full_update = false;
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

