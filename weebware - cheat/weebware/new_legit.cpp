#include "Header.h"
#include "new_legit.h"

c_triggerbot g_triggerbot;

void c_newlegit::run(c_usercmd* cmd, c_base_entity* local) {

	// we share local for all funcs and is never null, dont forget to sanatize weapons..
	set_local(local);

	// need to add param to incorporate aimbot angles, and reduction %
	// reduce_recoil(cmd);

	// TOO MANY FUNCS I MADE IT ITS OWN CLASS
	g_triggerbot.run(cmd, this);
}

void c_triggerbot::run(c_usercmd* cmd, c_newlegit* legit) {

	m_local = legit->get_local();

	switch (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_active) {
	case 0:
		return;
	case 1:
		if (!(GetAsyncKeyState(g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_key))) {
			m_last_delay = get_epoch_ms();
			last_delay_trig = 0;
			cur_target = NULL;
			return;
		}
		break;
	}

	c_base_entity* target = get_trace_ent();

	// our trace didnt return an ent.. lets reset our last delay and do no more
	if (!target) {
		m_last_delay = get_epoch_ms();
		return;
	}

	// we have an entity, lets shoot
	shoot(cmd, target);

}


c_base_entity* c_triggerbot::get_trace_ent() {

	// get our viewangles and account for aimpunch
	g_weebware.g_engine->get_view_angles(view_angles);
	view_angles += m_local->m_aimPunchAngle() * 2.f;

	// this will be our ray
	Vector forward = Vector(0, 0, 0);
	g_maths.qangle_vector(view_angles, forward);
	Vector dst = m_local->get_vec_eyepos() + (forward * m_local->m_pActiveWeapon()->get_weapon_info()->flRange);

	trace_t trace;
	Ray_t ray;
	Vector src = m_local->get_vec_eyepos();

	ray.Init(src, dst);

	ITraceFilter traceFilter;
	traceFilter.pSkip = (void*)m_local;

	g_weebware.g_engine_trace->TraceRay(ray, 0x46004003, &traceFilter, &trace);

	c_base_entity* trace_entity = trace.m_pEnt;

	if (!trace_entity->is_valid_player())
		return nullptr;

	if (trace_entity->m_iTeamNum() == m_local->m_iTeamNum())
		return nullptr;

	std::vector<int>hitboxes;

	for (int i = 1; i <= 7; i++) {
		hitboxes.push_back(i);
	}

	for (auto hitbox : hitboxes)
	{
		if (trace.hitgroup == hitbox) {
			return trace_entity;
		}
	}
	return nullptr;
}

void c_triggerbot::shoot(c_usercmd* cmd, c_base_entity* target) {

	if (get_epoch_ms() < (m_last_delay + g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_reaction))
		return;


	if (g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_scoped_only)
		if (!sniper_scoped())
			return;


	if (!g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_aim_through_smoke) {
		if (target->trace_from_smoke(*m_local->m_vecOrigin()))
			return;
	}

	if (raytrace_hc(view_angles, g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_hitchance, target, m_local->m_pActiveWeapon()->get_weapon_info()->flRange) && next_attack_queued()) {
		cmd->buttons |= in_attack;
	}
	else {
		cmd->buttons &= ~in_attack;
	}
}


bool c_triggerbot::raytrace_hc(Vector viewAngles, float chance, c_base_entity* target, float dst)
{
	if (chance <= 0.f) return true;
	int hitCount = 0;
	Vector vecDirShooting, vecRight, vecUp;
	g_maths.qangle_vector(viewAngles, vecDirShooting, vecRight, vecUp);
	c_trace_customfilter traceFilter;

	for (int seed = 0; seed < 255; seed++)
	{
		Vector spread = get_spread(m_local->m_pActiveWeapon(), seed);

		Vector vecDir(vecDirShooting.x + spread.x * vecRight.x + spread.y * vecUp.x,
			vecDirShooting.y + spread.x * vecRight.y + spread.y * vecUp.y,
			vecDirShooting.z + spread.x * vecRight.z + spread.y * vecUp.z);

		Vector vecEnd = m_local->get_vec_eyepos() + (vecDir * dst);
		trace_t trace;
		Ray_t ray;
		traceFilter.pTarget = (void*)target;
		ray.Init(m_local->get_vec_eyepos(), vecEnd);
		g_weebware.g_engine_trace->TraceRay(ray, 0x4600400B, &traceFilter, &trace);

		if (trace.fraction == 1.0f)
			continue;

		if (trace.m_pEnt == target)
			hitCount++;
	}

	if (hitCount >= (255 * (chance / 100.0f)))
		return true;

	return false;
}

void c_newlegit::reduce_recoil(c_usercmd* cmd) {

		if (!m_local->m_iShotsFired() > 1)
			return;
	
		static QAngle old_punch = QAngle(0, 0, 0);
	
		QAngle view_angle = cmd->viewangles;
		QAngle punch = m_local->m_aimPunchAngle() * 2.f;
		QAngle rcs_angle = view_angle - (punch - old_punch);
		QAngle delta = g_maths.calcute_delta(view_angle, rcs_angle, 100);
		
		if (cmd->buttons & in_attack) {
			cmd->viewangles = delta;
			g_weebware.g_engine->set_view_angles(cmd->viewangles);
		}

		old_punch = punch;
}


// add these to the combatweapon class to make better..

bool c_triggerbot::sniper_scoped() {

	auto weapon = m_local->m_pActiveWeapon();

	if (!weapon)
		return false;

	if (weapon->is_scoped_weapon())
		if (m_local->m_bIsScoped())
			return true;

	return false;
}

bool c_triggerbot::next_attack_queued() {

	auto weapon = m_local->m_pActiveWeapon();

	if (!weapon)
		return false;

	if (weapon->m_flNextPrimaryAttack() > (float)g_weebware.g_global_vars->curtime)
		return false;

	return true;
}