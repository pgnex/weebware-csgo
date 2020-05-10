#include "backtrack.h"

void c_backtrack::init(c_usercmd* cmd) {

	c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
	if (!local)
		return;

	m_local = local;

	register_tick(cmd);

}

void c_backtrack::register_tick(c_usercmd* cmd) {

	ticks.insert(ticks.begin(), BacktrackTick{ cmd->tick_count });
	auto& cur = ticks[0];

	while (ticks.size() > 12)
		ticks.pop_back();

	for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++) {
		c_base_entity* cur_entity = (c_base_entity*)g_weebware.g_entlist->getcliententity(i);

		if (!cur_entity->is_valid_player())
			continue;

		if (cur_entity->m_iTeamNum() == m_local->m_iTeamNum())
			continue;

		cur.records.emplace_back(BacktrackRecord{ cur_entity, cur_entity->get_bone(8), cur_entity->m_vecOriginVal() });
	}
}



//c_base_entity* entity = nullptr;
//c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
//
//if (!local)
//return;
//
//float server_time = local->get_tick_base() * g_weebware.g_global_vars->interval_per_tick;
//
//c_basecombat_weapon* weapon = local->m_pActiveWeapon();
//
//if (!weapon)
//return;
