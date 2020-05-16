#include "Header.h"
#include "Legit.h"
#include "esp.h"

c_legitbot g_legitbot;

c_base_entity* cur_target = NULL;
c_base_entity* cur_target_trig = NULL;
int last_delay = 0, last_delay_trig = 0;
void c_legitbot::create_move(c_usercmd* cmd)
{
	g_weapon = m_local->m_pActiveWeapon();

	if (!g_weapon || g_weapon == nullptr)
		return;

	if (!g_weapon->is_firearm())
		return;

	triggerbot_main(cmd);

	standalone_rcs(cmd);

	if (!g_weebwarecfg.legit_cfg[get_config_index()].enable_legitbot)
		return;

	switch (g_weebwarecfg.legit_cfg[get_config_index()].enable_legitbot)
	{
	case 0:
		return;
	case 1:
		if (!(cmd->buttons & in_attack)) {
			m_last_time = get_epoch_ms();
			last_delay = 0.f;
			cur_target = NULL;
			return;
		}
		break;
	case 2:
		if (!GetAsyncKeyState(g_weebwarecfg.legit_cfg[get_config_index()].legitbot_key)) {
			m_last_time = get_epoch_ms();
			last_delay = 0.f;
			cur_target = NULL;
			return;
		}
		break;
	}

	if (m_local->is_flashed() && (!g_weebwarecfg.legit_cfg[get_config_index()].aim_while_blind))
		return;

	c_base_entity* target = closest_target_available();

	if (cur_target != target)
		if (cur_target != NULL)
			if (get_epoch_ms() <= (last_delay + g_weebwarecfg.legit_cfg[get_config_index()].aimbot_target_switch_delay))
				return;

	last_delay = get_epoch_ms();
	cur_target = target;

	if (!target->is_valid_player()) {
		last_delay = 0.f;
		cur_target = NULL;
		m_last_time = get_epoch_ms();
		return;
	}

	if (!is_visible(m_local, target))
		return;

	if (target->trace_from_smoke(*m_local->m_vecOrigin()) && (!g_weebwarecfg.legit_cfg[get_config_index()].aimbot_through_smoke))
		return;


	if (!(get_epoch_ms() > (m_last_time + g_weebwarecfg.legit_cfg[get_config_index()].reaction_time))) {
		return;
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].quick_stop)
		auto_stop(cmd);

	QAngle aim_angle = closest_hitbox(target);

	if (aim_angle.x == 0 && aim_angle.y == 0)
		return;


	float rcs_power = g_weebwarecfg.legit_cfg[get_config_index()].standalone_rcs_power;
	if (g_weebwarecfg.legit_cfg[get_config_index()].standalone_rcs) {
		if (rcs_power > g_weebwarecfg.legit_cfg[get_config_index()].aimbot_pitch_rcs || rcs_power > g_weebwarecfg.legit_cfg[get_config_index()].aimbot_yaw_rcs) {
			aim_angle = rcs_scaled(aim_angle, rcs_power, rcs_power);
		}
	}
	else {
		aim_angle = rcs_scaled(aim_angle, g_weebwarecfg.legit_cfg[get_config_index()].aimbot_pitch_rcs, g_weebwarecfg.legit_cfg[get_config_index()].aimbot_yaw_rcs);
	}

	g_maths.normalize_angle(aim_angle);

	g_maths.clamp_angle(aim_angle);

	QAngle view_angles = QAngle(0.f, 0.f, 0.f);
	g_weebware.g_engine->get_view_angles(view_angles);

	if (!g_weebwarecfg.legit_cfg[get_config_index()].silent_aim)
	{
		QAngle delta = g_maths.calcute_delta(view_angles, aim_angle, g_weebwarecfg.legit_cfg[get_config_index()].aim_sensitivity);

		cmd->viewangles = delta;

		g_weebware.g_engine->set_view_angles(cmd->viewangles);
	}
	else {

		if ((cmd->buttons & in_attack) && (next_attack_queued()))
			cmd->viewangles = aim_angle;
	}
}



c_base_entity* c_legitbot::closest_target_available()
{
	float best_fov = g_weebwarecfg.legit_cfg[get_config_index()].aimbot_fov;
	float closest_fov = 180.f;

	c_base_entity* best_entity = nullptr;
	c_base_entity* closest_ent = nullptr;

	for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++)
	{
		c_base_entity * cur_entity = g_weebware.g_entlist->getcliententity(i);

		if (!cur_entity->is_valid_player())
			continue;

		if (cur_entity->m_iTeamNum() == m_local->m_iTeamNum() && !g_weebwarecfg.legit_cfg[get_config_index()].target_teammates)
			continue;

		if (!is_visible(m_local, cur_entity))
			continue;

		Vector center_head = center_hitbox(cur_entity, (int)csgohitboxid::head);

		QAngle angle_to_head;

		g_maths.vector_qangles(center_head - m_local->get_vec_eyepos(), angle_to_head);

		g_maths.normalize_angle(angle_to_head);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);

		g_weebware.g_engine->get_view_angles(view_angles);

		view_angles += m_local->m_aimPunchAngle() * 2.f;

		// a bug with dstance fov is it can calculate the delta of someone behind you to be the of a lower fov.
		float this_fov = g_maths.get_fov(view_angles, angle_to_head, false, Vector(m_local->get_vec_eyepos() - center_head).size());
		float normal_fov = g_maths.get_fov(view_angles, angle_to_head);

		if (normal_fov < closest_fov) {
			closest_ent = cur_entity;
			closest_fov = normal_fov;
		}
		
		// prevent symmetry collision
		if (this_fov < g_weebwarecfg.legit_cfg[get_config_index()].aimbot_fov && this_fov < best_fov && closest_ent == cur_entity)
		{
			best_entity = cur_entity;
			best_fov = this_fov;
		}

	}

	return best_entity;
}

Vector c_legitbot::center_hitbox(c_base_entity* ent, int id)
{
	if (!ent)
	{
		return Vector(0, 0, 0);
	}

	matrix3x4_t matrix[128];

	if (!ent->setup_bones(matrix, 128, 0x00000100, g_weebware.g_global_vars->curtime))
	{
		return Vector(0, 0, 0); 
	}

	const model_t* mod = ent->get_model();

	if (!mod)
	{
		return Vector(0, 0, 0);
	}

	studiohdr_t* hdr = g_weebware.g_model_info->getstudiomodel(mod);

	if (!hdr)
	{
		return Vector(0, 0, 0);
	}

	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

	if (!set)
	{
		return Vector(0, 0, 0);
	}

	mstudiobbox_t* hitbox = set->pHitbox(id);

	if (!hitbox)
	{
		return Vector(0, 0, 0);
	}
	Vector vMin, vMax, vCenter;

	g_maths.VectorTransform2(hitbox->bbmin, matrix[hitbox->bone], vMin);

	g_maths.VectorTransform2(hitbox->bbmax, matrix[hitbox->bone], vMax);

	vCenter = (vMin + vMax) * 0.5f;

	return vCenter;
}


bool c_legitbot::is_visible_angle(c_base_entity* target, Vector dst2)
{
	trace_t Trace;

	Vector src = m_local->get_vec_eyepos(); 

	Ray_t ray;

	ray.Init(src, dst2);

	ITraceFilter traceFilter;

	traceFilter.pSkip = (void*)m_local;

	g_weebware.g_engine_trace->TraceRay(ray, MASK_SHOT, &traceFilter, &Trace);

	if (Trace.m_pEnt == target)
		return true;

	if (Trace.m_pEnt->is_valid_player())
		return true;

	if (Trace.fraction == 1.0f)
		return true;

	return false;
}


QAngle c_legitbot::magnet_hitbox(c_base_entity* target)
{
	if (!target->is_valid_player())
		return QAngle(0, 0, 0);

	std::vector<int> aim_spots;

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_head)
	{
		aim_spots.push_back(csgohitboxid::head);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_chest)
	{
		aim_spots.push_back(csgohitboxid::chest);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_stomach)
	{
		aim_spots.push_back(csgohitboxid::stomach);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_arms)
	{
		aim_spots.push_back(csgohitboxid::left_forearm);
		aim_spots.push_back(csgohitboxid::left_upper_arm);
		aim_spots.push_back(csgohitboxid::right_forearm);
		aim_spots.push_back(csgohitboxid::right_upper_arm);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_legs)
	{
		aim_spots.push_back(csgohitboxid::left_thigh);
		aim_spots.push_back(csgohitboxid::right_thigh);
		aim_spots.push_back(csgohitboxid::left_calf);
		aim_spots.push_back(csgohitboxid::right_calf);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_all)
	{
		aim_spots.push_back(csgohitboxid::head);
		aim_spots.push_back(csgohitboxid::chest);
		aim_spots.push_back(csgohitboxid::stomach);
		aim_spots.push_back(csgohitboxid::left_thigh);
		aim_spots.push_back(csgohitboxid::right_thigh);
		aim_spots.push_back(csgohitboxid::left_calf);
		aim_spots.push_back(csgohitboxid::right_calf);
	}

	float marginal_fov = 180;

	QAngle closest_hitbox = QAngle(0, 0, 0);
	Vector best_hitbox;
	QAngle view_angles = QAngle(0.f, 0.f, 0.f);

	for (auto potential_hitbox : aim_spots)
	{
		Vector hitbox = center_hitbox(target, potential_hitbox);

		QAngle angle_to_hitbox;

		g_maths.vector_qangles(hitbox - m_local->get_vec_eyepos(), angle_to_hitbox);

		g_weebware.g_engine->get_view_angles(view_angles);

		float this_fov = g_maths.get_fov(view_angles, angle_to_hitbox);

		if (this_fov < marginal_fov)
		{
			closest_hitbox = angle_to_hitbox;

			best_hitbox = hitbox;

			marginal_fov = this_fov;
		}
	}

	if (!is_visible_angle(target, best_hitbox))
		return view_angles;

	return closest_hitbox;
}

QAngle c_legitbot::closest_hitbox(c_base_entity* target)
{
	if (!target->is_valid_player())
		return QAngle(0, 0, 0);

	std::vector<int> aim_spots;

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_all)
	{
		for (int i = 0; i != csgohitboxid::max; i++) {
			aim_spots.push_back(static_cast<csgohitboxid>(i));
		}
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_legs)
	{
		aim_spots.push_back(csgohitboxid::right_thigh);
		aim_spots.push_back(csgohitboxid::left_thigh);
		aim_spots.push_back(csgohitboxid::right_calf);
		aim_spots.push_back(csgohitboxid::left_calf);
		aim_spots.push_back(csgohitboxid::right_foot);
		aim_spots.push_back(csgohitboxid::left_foot);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_arms)
	{
		aim_spots.push_back(csgohitboxid::right_hand);
		aim_spots.push_back(csgohitboxid::left_hand);
		aim_spots.push_back(csgohitboxid::right_upper_arm);
		aim_spots.push_back(csgohitboxid::right_forearm);
		aim_spots.push_back(csgohitboxid::left_upper_arm);
		aim_spots.push_back(csgohitboxid::left_forearm);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_head)
	{
		aim_spots.push_back(csgohitboxid::head);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_chest)
	{
		aim_spots.push_back(csgohitboxid::chest);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].aimbot_stomach)
	{
		aim_spots.push_back(csgohitboxid::stomach);
	}

	float marginal_fov = 180;

	QAngle closest_hitbox = QAngle(0, 0, 0);

	for (auto potential_hitbox : aim_spots)
	{
		Vector hitbox = center_hitbox(target, potential_hitbox);

		QAngle angle_to_hitbox;

		g_maths.vector_qangles(hitbox - m_local->get_vec_eyepos(), angle_to_hitbox);

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

QAngle c_legitbot::rcs_scaled(QAngle original_angle, float pitch, float yaw)
{
	QAngle delta = original_angle;

	if (!m_local->m_iShotsFired() > 1)
		return original_angle;

	delta.x -= (m_local->m_aimPunchAngle().x * (2.f / 100.f * pitch));

	delta.y -= (m_local->m_aimPunchAngle().y * (2.f / 100.f * yaw));

	return delta;
}


void c_legitbot::standalone_rcs(c_usercmd* cmd)
{
	if (!g_weebwarecfg.legit_cfg[get_config_index()].standalone_rcs)
		return;

	if (!m_local->m_iShotsFired() > 1)
		return;

	static QAngle old_punch = QAngle(0, 0, 0);

	QAngle view_angle = cmd->viewangles;

	QAngle punch = m_local->m_aimPunchAngle() * 2.f;

	QAngle rcs_angle = view_angle - (punch - old_punch);

	QAngle delta = g_maths.calcute_delta(view_angle, rcs_angle, g_weebwarecfg.legit_cfg[get_config_index()].standalone_rcs_power);

	g_maths.normalize_angle(delta);

	g_maths.clamp_angle(delta);

	if (cmd->buttons & in_attack) {

		cmd->viewangles = delta;

		g_weebware.g_engine->set_view_angles(cmd->viewangles);
	}

	old_punch = punch;
}


bool c_legitbot::raytrace_hc(Vector viewAngles, float chance, c_base_entity* target, float dst)
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

bool c_legitbot::next_attack_queued()
{
	if (m_local->m_pActiveWeapon()->m_flNextPrimaryAttack() > (float)g_weebware.g_global_vars->curtime)
		return false;
	return true;
}
#if 1
void c_legitbot::auto_stop(c_usercmd* cmd)
{
	cmd->forwardmove = 0;
	cmd->sidemove = 0;
}
#endif



c_base_entity* c_legitbot::closest_target_triggerbot()
{
	float best_fov = g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_fov;

	float closest_fov = 180.f;

	c_base_entity* best_entity = nullptr;
	c_base_entity* closest_ent = nullptr;
	// std::vector<int> hitboxes = get_triggerbot_hitboxes();

	for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++)
	{
		c_base_entity* cur_entity = g_weebware.g_entlist->getcliententity(i);

		if (!cur_entity->is_valid_player())
			continue;

		if (cur_entity->m_iTeamNum() == m_local->m_iTeamNum())
			continue;

		Vector center_head = center_hitbox(cur_entity, (int)csgohitboxid::head);

		QAngle angle_to_head;

		g_maths.vector_qangles(center_head - m_local->get_vec_eyepos(), angle_to_head);

		g_maths.normalize_angle(angle_to_head);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);

		g_weebware.g_engine->get_view_angles(view_angles);

		view_angles += m_local->m_aimPunchAngle() * 2.f;

		// a bug with dstance fov is it can calculate the delta of someone behind you to be the of a lower fov.
		float this_fov = g_maths.get_fov(view_angles, angle_to_head, 0, Vector(m_local->get_vec_eyepos() - center_head).size());

		float normal_fov = g_maths.get_fov(view_angles, angle_to_head);

		if (normal_fov < closest_fov) {
			closest_ent = cur_entity;
			closest_fov = normal_fov;
		}

		// prevent symmetry collision
		if (this_fov < g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_fov && this_fov < best_fov && closest_ent == cur_entity)
		{
			best_entity = cur_entity;
			best_fov = this_fov;
		}

	}

	return best_entity;
}

void c_legitbot::magnet_triggerbot(c_usercmd* cmd) {

	if (!g_weebwarecfg.legit_cfg[get_config_index()].magnet_triggerbot_enabled)
		return;

	c_base_entity* target = closest_target_triggerbot();

	if (!target->is_valid_player())
		return;

	if (!is_visible(m_local, target))
		return;

	if (cur_target_trig != target)
		if (cur_target_trig != NULL)
			if (get_epoch_ms() <= (last_delay_trig + g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_target_switch_delay))
				return;

	last_delay_trig = get_epoch_ms();
	cur_target_trig = target;

	if (target) {

		if (g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].quick_stop_magnet)
			auto_stop(cmd);

		auto aim_angle = magnet_hitbox(target);

		g_maths.normalize_angle(aim_angle);
		
		g_maths.clamp_angle(aim_angle);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);
		g_weebware.g_engine->get_view_angles(view_angles);

		QAngle delta = g_maths.calcute_delta(view_angles, aim_angle, g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_sensitivity);

		cmd->viewangles = delta;

		if (g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_scoped_only)
			if (!sniper_scoped())
				return;


		if (!g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_through_smoke) {
			if (target->trace_from_smoke(*m_local->m_vecOrigin()))
				return;
		}


		g_weebware.g_engine->set_view_angles(cmd->viewangles);
	}
}

bool c_legitbot::sniper_scoped() {

	auto weapon = m_local->m_pActiveWeapon();

	if (!weapon)
		return false;

	if (weapon->is_scoped_weapon())
		if (m_local->m_bIsScoped())
			return true;

	return false;
}

void c_legitbot::triggerbot_main(c_usercmd* cmd)
{
	static float m_last_delay = 0.f;

	switch (g_weebwarecfg.legit_cfg[get_config_index()].enable_triggerbot) {
	case 0:
		return;
	case 1:
		if (!(GetAsyncKeyState(g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_key))) {
			m_last_delay = get_epoch_ms();
			last_delay_trig = 0;
			cur_target_trig = NULL;
			return;
		}
		break;
	}

	magnet_triggerbot(cmd);

	//g_legitbot.g_local
	QAngle view_angles = QAngle(0.f, 0.f, 0.f);

	g_weebware.g_engine->get_view_angles(view_angles);

	view_angles += m_local->m_aimPunchAngle() * 2.f;

	Vector forward = Vector(0, 0, 0);

	g_maths.qangle_vector(view_angles, forward);

	// The ray
	forward = m_local->get_vec_eyepos() + (forward * m_local->m_pActiveWeapon()->get_weapon_info()->flRange);

	c_basecombat_weapon* weapon = m_local->m_pActiveWeapon();
	c_weapon_info* info = weapon->get_weapon_info();

	trace_t trace;

	Vector src = m_local->get_vec_eyepos(), dst = forward;

	Ray_t ray;

	ray.Init(src, dst);

	ITraceFilter traceFilter;

	traceFilter.pSkip = (void*)m_local;

	g_weebware.g_engine_trace->TraceRay(ray, 0x46004003, &traceFilter, &trace);

	c_base_entity* trace_entity = trace.m_pEnt;

	if (!trace_entity->is_valid_player())
		return;

	if (trace_entity->m_iTeamNum() == m_local->m_iTeamNum())
		return;

	std::vector<int>hitboxes;


	if (g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_all) {
		for (int i = 1; i <= 7; i++) {
			hitboxes.push_back(i);
		}
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

	bool has_hitgroup = false;

	for (auto hitbox : hitboxes)
	{
		if (trace.hitgroup == hitbox) {
			has_hitgroup = true;
		}
	}

	static int queue_shot = 0;

	if (has_hitgroup) {

		if (get_epoch_ms() <= (m_last_delay + g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_reaction))
			return;	

		if (g_weebwarecfg.legit_cfg[g_weebwarecfg.legit_cfg_index].triggerbot_scoped_only)
			if (!sniper_scoped())
				return;


		if (!g_weebwarecfg.legit_cfg[get_config_index()].mag_trig_through_smoke) {
			if (trace_entity->trace_from_smoke(*m_local->m_vecOrigin()))
				return;
		}
		

		if (raytrace_hc(view_angles, g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_hitchance, trace_entity, m_local->m_pActiveWeapon()->get_weapon_info()->flRange) && next_attack_queued()) {
			cmd->buttons |= in_attack;
		}
		else {
			cmd->buttons &= ~in_attack;
		}
	}
	else {
		m_last_delay = get_epoch_ms();

	}


}



