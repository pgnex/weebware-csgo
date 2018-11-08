#include "Header.h"
#include "shared.h"
#include "skinchanger.h"

void knife_changer::frame_stage_notify() {
	// make sure a knife is selected 
	if (!g_weebwarecfg.selected_knife_index > 0)
		return;
	// connected and in game check
	if (!g_weebware.g_engine->is_connected() || !g_weebware.g_engine->is_in_game())
		return;
	// local player check
	player_info local_player_info;
	int local_player_index = g_weebware.g_engine->get_local();
	c_base_entity* local_player = g_weebware.g_entlist->getcliententity(local_player_index);
	if (!local_player || local_player->m_iHealth() <= 0 || !g_weebware.g_engine->GetPlayerInfo(local_player_index, &local_player_info))
		return;

	int *weapons = local_player->get_weapons();
	if (!weapons)
		return;

	for (size_t i = 0; weapons[i] != 0xFFFFFFFF; i++) {
		// get weapon
		HANDLE wep_handle = (HANDLE)weapons[i];
		c_basecombat_weapon *weapon = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(wep_handle));
		// nullptr check
		if (!weapon)
			continue;
		// make sure the weapon is a knife
		if (!weapon->is_knife())
			continue;

		auto vm_handle = local_player->get_viewmodel_handle();

		if (!vm_handle)
			continue;

		auto viewmodel = reinterpret_cast<c_viewmodel*>(g_weebware.g_entlist->getcliententityfromhandle(vm_handle));

		if (!viewmodel)
			continue;

		auto viewmodel_weapon = reinterpret_cast<c_basecombat_weapon*>(g_weebware.g_entlist->getcliententityfromhandle(viewmodel->get_weapon_handle()));

		int model_index = g_weebware.g_model_info->getmodelindex("models/weapons/v_knife_push.mdl");
		*viewmodel->m_nModelIndex() = model_index;
		*weapon->m_nModelIndex() = model_index;
		*weapon->m_nViewModelIndex() = model_index;
		*weapon->get_fallbackwear() = 1;
		*weapon->m_iItemDefinitionIndexPtr() = 516;
		std::cout << "set" << std::endl;
	}
}