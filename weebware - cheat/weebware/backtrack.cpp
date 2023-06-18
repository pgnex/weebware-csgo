#include "Header.h"
#include "backtrack.h"
#include "Legit.h"

#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / g_weebware.g_global_vars->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( g_weebware.g_global_vars->interval_per_tick *( t ) )
#define ROUND_TO_TICKS( t )		( g_weebware.g_global_vars->interval_per_tick * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK		(-1)

template< class T, class Y >
T fClamp(T const& val, Y const& minVal, Y const& maxVal)
{
	if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}


c_backtrack::c_accuracy_records c_backtrack::create_record(c_base_entity* entity, c_usercmd* cmd) {
	c_accuracy_records cur_record;
	cur_record.m_simulation_time = *entity->m_flSimulationTime();
	cur_record.m_head = entity->get_bone(8);
	cur_record.record_tick = cmd->tick_count;
	cur_record.index = entity->EntIndex();
	cur_record.visible = is_visible(m_local, entity);

	// no skeleton, no point looping bones.
	if (!g_weebwarecfg.visuals_backtrack_skeleton)
		return cur_record;

	auto studiomodel = g_weebware.g_model_info->getstudiomodel(entity->getmodel());

	if (studiomodel) {

		cur_record.bonecount = studiomodel->numbones;

		if (cur_record.bonecount) {

			for (int i = 0; i < cur_record.bonecount; i++)
			{
				mstudiobone_t* pBone = studiomodel->GetBone(i);

				if (pBone && (pBone->flags & 256) && (pBone->parent != -1))
				{
					cur_record.child[i] = entity->get_bone(i);
					cur_record.parent[i] = entity->get_bone(pBone->parent);
				}
			}
		}
	}
	return cur_record;
}


bool is_valid_tick(float sim) {
	float correct = 0;

	correct += g_weebware.g_engine->get_net_channel()->GetLatency(0);
	correct += g_weebware.g_engine->get_net_channel()->GetLatency(1);

	c_convar* cl_updaterate = g_weebware.g_convars->find_cvar("cl_updaterate");
	float lerp = g_weebware.g_convars->find_cvar("cl_interp")->GetFloat();
	c_convar* cl_interp_ratio = g_weebware.g_convars->find_cvar("cl_interp_ratio");

	float m_flLerpTime = max(lerp, cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat());

	correct += m_flLerpTime;

	correct = fClamp(correct, 0.f, 1.f);

	float deltaTime = correct - (g_weebware.g_global_vars->curtime - sim);

	if (fabsf(deltaTime) > 0.2f)
	{
		return false;
	}

	return true;
}

bool c_backtrack::is_valid_record(c_accuracy_records record) {
	if (!is_valid_tick(record.m_simulation_time))
		return false;
	
	return true;
}

void c_backtrack::clear_all_records() {
	accuracy_records.clear();
}

void c_backtrack::accuracy_boost(c_usercmd* cmd) {
	
	m_local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!g_weebwarecfg.legit_cfg[get_config_index()].accuracy_boost || !m_local || !m_local->m_pActiveWeapon() || !m_local->m_pActiveWeapon()->is_firearm()) {
		clear_all_records();
		return;
	}

	// create records
	for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++)
	{
		c_base_entity* cur_entity = g_weebware.g_entlist->getcliententity(i);

		if (!cur_entity->is_valid_player())
			continue;

		if (cur_entity == m_local)
			continue;

		if (cur_entity->m_iTeamNum() == m_local->m_iTeamNum())
			continue;

		if (cur_entity->m_bGunGameImmunity())
			continue;

		//if (cur_entity->is_stationary())
		//	continue;

		accuracy_records.push_back(create_record(cur_entity, cmd));
	}

	float marginal_fov = 20;

	for (size_t i = 0; i < accuracy_records.size(); i++)
	{
		bool ExistingPlayer = true;
		auto record_player = g_weebware.g_entlist->getcliententity(accuracy_records[i].index);

		if (!record_player || !record_player->is_valid_player())
			ExistingPlayer = false;

		if (!is_valid_record(accuracy_records[i]) || !ExistingPlayer
			|| ((cmd->tick_count - g_weebwarecfg.legit_cfg[get_config_index()].legit_maximum_ticks) >= accuracy_records[i].record_tick))
		{
			accuracy_records.erase(accuracy_records.begin() + i);
			continue;
		}

		QAngle angle_to_hitbox;

		g_maths.vector_qangles(accuracy_records[i].m_head - m_local->get_vec_eyepos(), angle_to_hitbox);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);

		g_weebware.g_engine->get_view_angles(view_angles);

		view_angles += m_local->m_aimPunchAngle() * 2.f;

		float this_fov = g_maths.get_fov(view_angles, angle_to_hitbox);

		if (this_fov < marginal_fov)
		{
			accuracy_records[i].m_best_record = true;
			m_best_record = accuracy_records[i];
			marginal_fov = this_fov;
		}


	}

	if (cmd->buttons & in_attack)
	{
		cmd->tick_count = m_best_record.record_tick;
	}
}
