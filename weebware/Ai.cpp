/*
CSGO Automated by Kite.
*/

#include "Header.h"
#include "Ai.h"
#include "drawing.h"
#include "Legit.h"

c_ai g_ai;

void c_ai::create_move(c_usercmd* cmd, c_base_entity* local)
{
	m_localview = cmd->viewangles;

	m_local = local;

	m_target_ent = nullptr;

	if (!g_weebwarecfg.misc_ai) {
		g_Walkbot.m_target_area = nullptr;
		return;
	}

	if (cmd->buttons & in_attack)
		return;

	if (g_weebwarecfg.misc_ai_random) {
		g_Walkbot.m_CurrentMode = ai_movement::random;
	}

	m_flags = ai_flags::standby;

	m_tempt_fov = 180.f;

	// We want ALL the entities
	for (auto i = 1; i <= g_weebware.g_entlist->getmaxentities(); i++)
	{
		c_base_entity* entity = g_weebware.g_entlist->getcliententity(i);

		if (!entity)
			continue;

		// We need to find if the planted bomb exists...
		if (strstr(entity->get_client_class()->m_networkedname, "CPlantedC4") && g_weebwarecfg.misc_ai_defuse)
		{
			float remaining = reinterpret_cast<c_bomb*>(entity)->get_blow_time() - g_weebware.g_global_vars->curtime;

			// We still have time, and bomb is active!
			if (remaining > 10.f && reinterpret_cast<c_bomb*>(entity)->is_ticking()) {

				// Lets set our entity.
				g_Walkbot.m_TargetEntity = entity;
				g_Walkbot.m_CurrentMode = ai_movement::defuse_bomb;
			}
			else {
				// No bomb...
				g_Walkbot.m_TargetEntity = nullptr;
			}

		}

		// Filter out players, we are done with other entities.

		if (!entity->is_valid_player())
			continue;

		// We don't want team mates

		if (entity->m_iTeamNum() == m_local->m_iTeamNum()) {
			continue;
		}

		if (entity->m_bGunGameImmunity())
			continue;

		// check for closest player VISIBLE.
		if (!is_visible(entity))
			continue;

		QAngle angle_to_head = QAngle(0.f, 0.f, 0.f);

		g_maths.vector_qangles(entity->get_bone(8) - m_local->get_vec_eyepos(), angle_to_head);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);

		g_weebware.g_engine->get_view_angles(view_angles);

		view_angles += m_local->m_aimPunchAngle() * 2.f;

		float this_fov = g_maths.get_fov(view_angles, angle_to_head);

		// Get the closest entity in range 
		if (this_fov < m_tempt_fov)
		{
			m_target_ent = entity;

			m_flags = ai_flags::aggravated;

			// we need a blatant factor
			if (this_fov <= 25.f) {
				m_flags = ai_flags::kill;
			}

			m_tempt_fov = this_fov;
		}
	}

	if (!m_target_ent->is_valid_player())
		m_target_ent = nullptr;

	if (m_target_ent && g_weebwarecfg.misc_ai_nearest) {

		if (m_flags == ai_flags::aggravated) {

			// walk to increase accuray for combat
			cmd->forwardmove = 130;

			correct_range(cmd);
		}
		else if (m_flags == ai_flags::kill) {
			this->kill(cmd);
		}
	}
	else {
		cmd->forwardmove = 450;

		// Just traversing
		adjust_to_velocity(cmd);

		jump_on_low_velocity(cmd);
	}

	// Lets get proper velocities

	// Detect if there was a player in map with a bomb.


	// We should almost never use random... its unreliable...

	// We need a constant 180 fov check. - if we detect a player rotate ourselves slowly and stop moving.

	// wait till we are within like 30~ degrees then sorta aimbot.

	// Detect if we are CT or T.
	g_Walkbot.create_move(cmd, local);
}

void c_ai::jump_on_low_velocity(c_usercmd* cmd)
{
	int tick_rate = 1 / g_weebware.g_global_vars->interval_per_tick;

	static int ticks_elpased = 0;

	static int jumps = 0;

	if (!(cmd->buttons & in_attack))
	{
		if (ticks_elpased > tick_rate) {
			// stuck fixes
			// m_pCurrentArea->m_attributeFlags & NAV_MESH_JUMP
			if (this->m_local->m_vecVelocity().size() <= 5.f) {
				cmd->buttons |= in_jump;
				jumps++;
			}

			ticks_elpased = 0;
		}
	}
	++ticks_elpased;

	if (jumps > 3) {
		// check if we are moving slower than normal
		if (this->m_local->m_vecVelocity().size() <= 25.f) {
		
			// Time to reset navmesh stuff
			g_Walkbot.m_target_area = nullptr;
			g_Walkbot.m_TargetEntity = nullptr;
		}
	}
}
void c_ai::adjust_to_velocity(c_usercmd* cmd)
{
	auto direction = m_local->m_vecVelocity();

	// Are we moving?

	if (direction.size() <= 0.1f) {
		return;
	}

	// Convert direction to angles...
	g_maths.vector_qangles(direction, direction);

	direction.x = 0;

	// We dont want to instanteously snap tho
	direction = g_legitbot.calcute_delta(cmd->viewangles, direction, 5.f);

	// Only yaw
	cmd->viewangles.y = direction.y;
	cmd->viewangles.x = direction.x;

	g_weebware.g_engine->set_view_angles(cmd->viewangles);
}

bool c_ai::is_visible(c_base_entity* target)
{
	trace_t Trace;

	Vector src = m_local->get_vec_eyepos(), dst2 = target->get_bone(8); // 8 is head. 

	Ray_t ray;

	ray.Init(src, dst2);

	ITraceFilter traceFilter;

	traceFilter.pSkip = reinterpret_cast<decltype(traceFilter.pSkip)>(m_local);

	g_weebware.g_engine_trace->TraceRay(ray, MASK_SHOT, &traceFilter, &Trace);

	if (Trace.m_pEnt == target)
		return true;

	if (Trace.fraction == 1.0f)
		return true;

	return false;
}

// Preferable only use assault rifles - most reliable.
void c_ai::auto_buy_weapons(c_usercmd* cmd)
{
	// Try and equip primary
	if (!(cmd->buttons & in_weapon1))
		cmd->buttons |= in_weapon1;

	if (m_local->m_pActiveWeapon()->is_firearm() || m_local->m_fFlags() & fl_frozen) {
		return;
	}
	// get current weapon

	std::string console_buffer = "buy ";

	bool b_ct = m_local->m_iTeamNum() == 1;

}


void c_ai::correct_range(c_usercmd* cmd) {

	Vector direction = g_legitbot.center_hitbox(m_target_ent, csgohitboxid::chest);

	QAngle aim_dir;
	g_maths.vector_qangles(direction - m_local->get_vec_eyepos(), aim_dir);

	// rotate player to visible range
	cmd->viewangles = g_legitbot.calcute_delta(m_localview, aim_dir, g_weebwarecfg.misc_ai_rotationspeed);

	g_weebware.g_engine->set_view_angles(cmd->viewangles);
}

void c_ai::kill(c_usercmd* cmd) {

	Vector direction = g_legitbot.center_hitbox(m_target_ent, csgohitboxid::head);

	QAngle aim_dir;

	g_maths.vector_qangles(direction - m_local->get_vec_eyepos(), aim_dir);

	aim_dir -= m_local->m_aimPunchAngle() * 2.f;

	cmd->viewangles = g_legitbot.calcute_delta(m_localview, aim_dir, g_weebwarecfg.misc_ai_aimspeed);

	g_weebware.g_engine->set_view_angles(cmd->viewangles);

	if (m_local->m_pActiveWeapon()->Clip1() != 0)
	{
		g_legitbot.auto_stop(cmd);

		if (m_local->m_pActiveWeapon()->m_flNextPrimaryAttack() <= (float)g_weebware.g_global_vars->curtime) {
			cmd->buttons |= in_attack;
		}
	}
	else
		cmd->buttons |= in_reload;
}

