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

	for (size_t i = 0; weapons[i] != 0xFFFFFFFF; i++)
	{
		auto weapon = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(reinterpret_cast<HANDLE>(weapons[i])));

		if (!weapon)
			continue;

		auto weapon_id = weapon->m_iItemDefinitionIndex();

		auto skin_config = g_weebwarecfg.skin_wheel[weapon_id];

		*weapon->get_item_id_high() = -1;

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
