#include "Header.h"
#include "Legit.h"

c_triggerbot g_triggerbot;
c_aimbot g_aimbot;

void c_legit::run(c_usercmd* cmd, c_base_entity* local) {

	// weapon check bc fk it
	if (!local->m_pActiveWeapon())
		return;

	g_legitbot.m_local = local;

	// need to add param to incorporate aimbot angles, and reduction %
	reduce_recoil(cmd);


	// TOO MANY FUNCS I MADE IT ITS OWN CLASS
	g_triggerbot.run(cmd);
	g_aimbot.run(cmd);
}

void c_legit::reduce_recoil(c_usercmd* cmd) {

	if (!g_weebwarecfg.legit_cfg[get_config_index()].standalone_rcs)
		return;

	if (cur_target != NULL)
		return;

	if (!g_legitbot.m_local->m_iShotsFired() > 1)
		return;

	static QAngle old_punch = QAngle(0, 0, 0);

	QAngle view_angle = cmd->viewangles;
	QAngle punch = g_legitbot.m_local->m_aimPunchAngle() * 2.f;
	QAngle rcs_angle = view_angle - (punch - old_punch);
	QAngle delta = g_maths.calcute_delta(view_angle, rcs_angle, g_weebwarecfg.legit_cfg[get_config_index()].standalone_rcs_power);

	if (cmd->buttons & in_attack) {
		cmd->viewangles = delta;
		g_weebware.g_engine->set_view_angles(cmd->viewangles);
	}

	old_punch = punch;
}

QAngle c_legit::rcs_scaled(QAngle original_angle, bool triggerbot) {
	QAngle delta = original_angle;

	float pitch = triggerbot ? g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_pitch_rcs : g_weebwarecfg.legit_cfg[get_config_index()].aimbot_pitch_rcs;
	float yaw = triggerbot ? g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_yaw_rcs : g_weebwarecfg.legit_cfg[get_config_index()].aimbot_yaw_rcs;

	if (!g_legitbot.m_local->m_iShotsFired() > 1)
		return original_angle;

	delta.x -= (g_legitbot.m_local->m_aimPunchAngle().x * (2.f / 100.f * pitch));

	delta.y -= (g_legitbot.m_local->m_aimPunchAngle().y * (2.f / 100.f * yaw));

	return delta;
}



///
///	AIMBOT STUFF
///

void c_aimbot::run(c_usercmd* cmd) {

	switch (g_weebwarecfg.legit_cfg[get_config_index()].enable_legitbot) {
	case 0:
		return;
	case 1:
		if (!(cmd->buttons & in_attack)) {
			m_last_delay = get_epoch_ms();
			last_delay_aim = 0.f;
			cur_target = NULL;
			return;
		}
		break;
	case 2:
		if (!GetAsyncKeyState(g_weebwarecfg.legit_cfg[get_config_index()].legitbot_key)) {
			m_last_delay = get_epoch_ms();
			last_delay_aim = 0.f;
			cur_target = NULL;
			return;
		}
		break;
	}

	if (g_legitbot.m_local->is_flashed() && (!g_weebwarecfg.legit_cfg[get_config_index()].aim_while_blind))
		return;

	do_aim_stuffs(cmd, false);
}

c_base_entity* c_legit::closest_target_available(bool triggerbot) {

	float best_fov = triggerbot ? g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_fov : g_weebwarecfg.legit_cfg[get_config_index()].aimbot_fov;
	float closest_fov = 180.f;

	c_base_entity* best_entity = nullptr;
	c_base_entity* closest_ent = nullptr;

	for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++)
	{
		c_base_entity* cur_entity = g_weebware.g_entlist->getcliententity(i);

		if (!cur_entity->is_valid_player())
			continue;

		if (cur_entity->m_iTeamNum() == g_legitbot.m_local->m_iTeamNum() && !g_weebwarecfg.legit_cfg[get_config_index()].target_teammates)
			continue;

		if (!is_visible(g_legitbot.m_local, cur_entity))
			continue;

		Vector center_head = center_hitbox(cur_entity, (int)csgohitboxid::head);

		QAngle angle_to_head;

		g_maths.vector_qangles(center_head - g_legitbot.m_local->get_vec_eyepos(), angle_to_head);

		g_maths.normalize_angle(angle_to_head);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);

		g_weebware.g_engine->get_view_angles(view_angles);

		view_angles += g_legitbot.m_local->m_aimPunchAngle() * 2.f;

		// a bug with dstance fov is it can calculate the delta of someone behind you to be the of a lower fov.
		float this_fov = g_maths.get_fov(view_angles, angle_to_head, false, Vector(g_legitbot.m_local->get_vec_eyepos() - center_head).size());
		float normal_fov = g_maths.get_fov(view_angles, angle_to_head);

		if (normal_fov < closest_fov) {
			closest_ent = cur_entity;
			closest_fov = normal_fov;
		}

		// prevent symmetry collision
		if (this_fov < best_fov && closest_ent == cur_entity)
		{
			best_entity = cur_entity;
			best_fov = this_fov;
		}

	}

	return best_entity;
}

void c_aimbot::do_aim_stuffs(c_usercmd* cmd, bool triggerbot) {

	if (g_legitbot.m_local->m_pActiveWeapon()->is_knife())
		return;

	c_base_entity* target = closest_target_available(triggerbot);

	int switch_delay = triggerbot ? g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_target_switch_delay : g_weebwarecfg.legit_cfg[get_config_index()].aimbot_target_switch_delay;

	if (cur_target != target && cur_target != NULL)
			if (get_epoch_ms() <= (last_delay_aim + switch_delay))
				return;

	last_delay_aim = get_epoch_ms();
	cur_target = target;

	if (!target->is_valid_player()) {
		last_delay_aim = 0.f;
		cur_target = NULL;
		m_last_delay = get_epoch_ms();
		return;
	}

	// maybe an improve accuracy func someday
	//if (g_weebwarecfg.legit_cfg[get_config_index()].aim_sensitivity > 85) {
	//	if (!in_crosshair(target) && g_legitbot.m_local->m_iShotsFired() == 0) {
	//		cmd->buttons &= ~in_attack;
	//	}
	//}

	if (!is_visible(g_legitbot.m_local, target))
		return;

	bool aim_through_smoke = triggerbot ? g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_through_smoke : g_weebwarecfg.legit_cfg[get_config_index()].aimbot_through_smoke;
	if (target->trace_from_smoke(*g_legitbot.m_local->m_vecOrigin()) && (!aim_through_smoke))
		return;


	bool autostop = triggerbot ? g_weebwarecfg.legit_cfg[get_config_index()].quick_stop_magnet : g_weebwarecfg.legit_cfg[get_config_index()].quick_stop;
	if (autostop)
		auto_stop(cmd);

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_scoped_only && triggerbot)
		if (!sniper_scoped())
			return;

	QAngle aim_angle = closest_hitbox(target, triggerbot);

	if (aim_angle.x == 0 && aim_angle.y == 0)
		return;


	// calculate aim angle with rcs incorperated
	aim_angle = rcs_scaled(aim_angle, triggerbot);
	g_maths.normalize_angle(aim_angle);
	g_maths.clamp_angle(aim_angle);

	QAngle view_angles = QAngle(0.f, 0.f, 0.f);
	g_weebware.g_engine->get_view_angles(view_angles);

	if (!g_weebwarecfg.legit_cfg[get_config_index()].silent_aim || triggerbot)
	{
		int sensitivity = triggerbot ? g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_sensitivity : g_weebwarecfg.legit_cfg[get_config_index()].aim_sensitivity;
		QAngle delta = g_maths.calcute_delta(view_angles, aim_angle, sensitivity);

		cmd->viewangles = delta;
		g_weebware.g_engine->set_view_angles(cmd->viewangles);
	}
	else {
			cmd->viewangles = aim_angle;
	}
}

bool c_aimbot::in_crosshair(c_base_entity* target) {

	if (!g_legitbot.m_local->m_pActiveWeapon()->is_scoped_weapon())
		return true;

	Ray_t ray;
	trace_t trace;
	Vector view_angles;

	g_weebware.g_engine->get_view_angles(view_angles);
	view_angles += g_legitbot.m_local->m_aimPunchAngle() * 2.f;

	// this will be our ray
	Vector forward = Vector(0, 0, 0);
	g_maths.qangle_vector(view_angles, forward);
	Vector dst = g_legitbot.m_local->get_vec_eyepos() + (forward * g_legitbot.m_local->m_pActiveWeapon()->get_weapon_info()->flRange);

	ray.Init(g_legitbot.m_local->get_vec_eyepos(), dst);
	ITraceFilter trace_filter;
	trace_filter.pSkip = g_legitbot.m_local;
	g_weebware.g_engine_trace->TraceRay(ray, 0x4600400B, &trace_filter, &trace);

	if (trace.allsolid || trace.startsolid)
		return false;

	if (!trace.m_pEnt)
		return false;

	if (trace.m_pEnt == target)
		return true;

	return false;
}

void c_aimbot::auto_stop(c_usercmd* cmd) {
	cmd->forwardmove = 0;
	cmd->sidemove = 0;
}

std::vector<int> c_aimbot::setup_hitboxes(bool triggerbot) {

	std::vector<int> aim_spots;

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_all && !triggerbot ||
		g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_all && triggerbot)
	{
		for (int i = 0; i != csgohitboxid::max; i++) {
			aim_spots.push_back(static_cast<csgohitboxid>(i));
		}
		return aim_spots;
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_legs && !triggerbot ||
		g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_legs && triggerbot)
	{
		aim_spots.push_back(csgohitboxid::right_thigh);
		aim_spots.push_back(csgohitboxid::left_thigh);
		aim_spots.push_back(csgohitboxid::right_calf);
		aim_spots.push_back(csgohitboxid::left_calf);
		aim_spots.push_back(csgohitboxid::right_foot);
		aim_spots.push_back(csgohitboxid::left_foot);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_arms && !triggerbot ||
		g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_arms && triggerbot)
	{
		aim_spots.push_back(csgohitboxid::right_hand);
		aim_spots.push_back(csgohitboxid::left_hand);
		aim_spots.push_back(csgohitboxid::right_upper_arm);
		aim_spots.push_back(csgohitboxid::right_forearm);
		aim_spots.push_back(csgohitboxid::left_upper_arm);
		aim_spots.push_back(csgohitboxid::left_forearm);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_head && !triggerbot ||
		g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_head && triggerbot)
	{
		aim_spots.push_back(csgohitboxid::head);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_chest && !triggerbot ||
		g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_chest && triggerbot)
	{
		aim_spots.push_back(csgohitboxid::chest);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_stomach && !triggerbot ||
		g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_stomach && triggerbot)
	{
		aim_spots.push_back(csgohitboxid::stomach);
	}

	return aim_spots;
}

QAngle c_aimbot::closest_hitbox(c_base_entity* target, bool triggerbot) {

	if (!target->is_valid_player())
		return QAngle(0, 0, 0);

	std::vector<int> aim_spots = setup_hitboxes(triggerbot);

	float marginal_fov = 180;

	QAngle closest_hitbox = QAngle(0, 0, 0);

	for (auto potential_hitbox : aim_spots)
	{
		Vector hitbox = center_hitbox(target, potential_hitbox);

		QAngle angle_to_hitbox;

		g_maths.vector_qangles(hitbox - g_legitbot.m_local->get_vec_eyepos(), angle_to_hitbox);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);

		g_weebware.g_engine->get_view_angles(view_angles);

		float this_fov = g_maths.get_fov(view_angles, angle_to_hitbox);

		if (this_fov < marginal_fov)
		{
			closest_hitbox = angle_to_hitbox;

			marginal_fov = this_fov;
		}
	}

	return closest_hitbox;
}



///
///	TRIGGERBOT STUFF
///

void c_triggerbot::run(c_usercmd* cmd) {

	switch (g_weebwarecfg.legit_cfg[get_config_index()].enable_triggerbot) {
	case 0:
		return;
	case 1:
		if (!(GetAsyncKeyState(g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_key))) {
			m_last_delay = get_epoch_ms();
			return;
		}
		break;
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].magnet_triggerbot_enabled)
		do_aim_stuffs(cmd, true);;

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
	view_angles += g_legitbot.m_local->m_aimPunchAngle() * 2.f;

	// this will be our ray
	Vector forward = Vector(0, 0, 0);
	g_maths.qangle_vector(view_angles, forward);
	Vector dst = g_legitbot.m_local->get_vec_eyepos() + (forward * g_legitbot.m_local->m_pActiveWeapon()->get_weapon_info()->flRange);

	trace_t trace;
	Ray_t ray;
	Vector src = g_legitbot.m_local->get_vec_eyepos();

	ray.Init(src, dst);

	ITraceFilter traceFilter;
	traceFilter.pSkip = (void*)g_legitbot.m_local;

	g_weebware.g_engine_trace->TraceRay(ray, 0x46004003, &traceFilter, &trace);

	c_base_entity* trace_entity = trace.m_pEnt;

	if (!trace_entity->is_valid_player())
		return nullptr;

	if (trace_entity->m_iTeamNum() == g_legitbot.m_local->m_iTeamNum())
		return nullptr;

	std::vector<int>hitboxes = setup_hitboxes();

	for (auto hitbox : hitboxes)
	{
		if (trace.hitgroup == hitbox) {
			return trace_entity;
		}
	}
	return nullptr;
}

std::vector<int> c_triggerbot::setup_hitboxes() {

	std::vector<int>hitboxes;

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_all) {
		for (int i = 1; i <= 7; i++) {
			hitboxes.push_back(i);
		}
		return hitboxes;
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_head)
		hitboxes.push_back(1);

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_chest)
		hitboxes.push_back(2);

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_stomach)
		hitboxes.push_back(3);

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_arms) {
		hitboxes.push_back(4);
		hitboxes.push_back(5);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_legs) {
		hitboxes.push_back(6);
		hitboxes.push_back(7);
	}

	return hitboxes;
}

void c_triggerbot::shoot(c_usercmd* cmd, c_base_entity* target) {

	if (get_epoch_ms() < (m_last_delay + g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_reaction))
		return;


	if (g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_scoped_only)
		if (!sniper_scoped())
			return;


	if (!g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_through_smoke) {
		if (target->trace_from_smoke(*g_legitbot.m_local->m_vecOrigin()))
			return;
	}

	if (raytrace_hc(view_angles, g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_hitchance, target, g_legitbot.m_local->m_pActiveWeapon()->get_weapon_info()->flRange) && next_attack_queued()) {
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
		Vector spread = get_spread(g_legitbot.m_local->m_pActiveWeapon(), seed);

		Vector vecDir(vecDirShooting.x + spread.x * vecRight.x + spread.y * vecUp.x,
			vecDirShooting.y + spread.x * vecRight.y + spread.y * vecUp.y,
			vecDirShooting.z + spread.x * vecRight.z + spread.y * vecUp.z);

		Vector vecEnd = g_legitbot.m_local->get_vec_eyepos() + (vecDir * dst);
		trace_t trace;
		Ray_t ray;
		traceFilter.pTarget = (void*)target;
		ray.Init(g_legitbot.m_local->get_vec_eyepos(), vecEnd);
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


bool c_legit::sniper_scoped() {

	auto weapon = g_legitbot.m_local->m_pActiveWeapon();

	if (!weapon)
		return false;

	if (weapon->is_scoped_weapon())
		if (g_legitbot.m_local->m_bIsScoped())
			return true;

	return false;
}

bool c_triggerbot::next_attack_queued() {

	auto weapon = g_legitbot.m_local->m_pActiveWeapon();

	if (!weapon)
		return false;

	if (weapon->m_flNextPrimaryAttack() > (float)g_weebware.g_global_vars->curtime)
		return false;

	return true;
}

bool c_aimbot::next_attack_queued() {

	auto weapon = g_legitbot.m_local->m_pActiveWeapon();

	if (!weapon)
		return false;

	if (weapon->m_flNextPrimaryAttack() > (float)g_weebware.g_global_vars->curtime)
		return false;

	return true;
}