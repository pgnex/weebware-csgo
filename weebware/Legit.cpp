#include "Legit.h"
#include "esp.h"

c_legitbot g_legitbot;

c_legitbot::c_accuracy_boost g_accuracy;

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
			m_last_time = get_epoch();
			return;
		}
		break;
	case 2:
		if (!GetAsyncKeyState(g_weebwarecfg.legit_cfg[get_config_index()].legitbot_activation_key)) {
			m_last_time = get_epoch();
			return;
		}
		break;
	}

	if (m_local->is_flashed() && (!g_weebwarecfg.legit_cfg[get_config_index()].aim_while_blind))
		return;


	c_base_entity* target = closest_target_available();

	if (!target->is_valid_player()) {
		m_last_time = get_epoch();
		return;
	}

	if (!is_visible(target))
		return;

	if (target->trace_from_smoke(*m_local->m_Origin()) && (!g_weebwarecfg.legit_cfg[get_config_index()].aim_through_smoke))
		return;


	if (!(get_epoch() > (m_last_time + g_weebwarecfg.legit_cfg[get_config_index()].reaction_time))) {
		return;
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].quick_stop)
		auto_stop(cmd);

	QAngle aim_angle = closest_hitbox(target);

	if (aim_angle.x == 0 && aim_angle.y == 0)
		return;

	aim_angle = rcs_scaled(aim_angle);

	g_maths.normalize_angle(aim_angle);

	g_maths.clamp_angle(aim_angle);

	QAngle view_angles = QAngle(0.f, 0.f, 0.f);
	g_weebware.g_engine->get_view_angles(view_angles);

	if (!g_weebwarecfg.legit_cfg[get_config_index()].silent_aim)
	{
		QAngle delta = calcute_delta(view_angles, aim_angle, g_weebwarecfg.legit_cfg[get_config_index()].sensitivity);

		cmd->viewangles = delta;

		g_weebware.g_engine->set_view_angles(cmd->viewangles);
	}
	else {

		if ((cmd->buttons & in_attack) && (next_attack_queued()))
			cmd->viewangles = aim_angle;
	}
}

int c_legitbot::get_config_index()
{
	if (g_weapon->is_pistol())
	{
		return 0;
	}
	else if (g_weapon->is_rifle())
	{
		return 1;
	}
	else if (g_weapon->is_smg())
	{
		return 2;
	}
	else if (g_weapon->is_shotgun())
	{
		return 3;
	}
	else if (g_weapon->is_heavy())
	{
		return 4;
	}
	else if (g_weapon->is_autosniper())
	{
		return 5;
	}
	else if (g_weapon->is_awp())
	{
		return 6;
	}
	else if (g_weapon->is_scout())
	{
		return 7;
	}

	// This shouldn't ever happen.
	return 0;
}

c_base_entity* c_legitbot::closest_target_available()
{

	float best_fov = g_weebwarecfg.legit_cfg[get_config_index()].maximum_fov;
	float closest_fov = 180.f;

	c_base_entity * best_entity = nullptr;
	c_base_entity * closest_ent = nullptr;

	for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++)
	{
		c_base_entity * cur_entity = g_weebware.g_entlist->getcliententity(i);

		if (!cur_entity->is_valid_player())
			continue;

		if (cur_entity->m_iTeamNum() == m_local->m_iTeamNum())
			continue;

		if (!is_visible(cur_entity))
			continue;

		Vector center_head = center_hitbox(cur_entity, (int)csgohitboxid::head);

		QAngle angle_to_head;

		g_maths.vector_qangles(center_head - m_local->get_vec_eyepos(), angle_to_head);

		g_maths.normalize_angle(angle_to_head);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);

		g_weebware.g_engine->get_view_angles(view_angles);

		view_angles += m_local->m_aimPunchAngle() * 2.f;

		// a bug with dstance fov is it can calculate the delta of someone behind you to be the of a lower fov.
		float this_fov = g_maths.get_fov(view_angles, angle_to_head, g_weebwarecfg.use_dynamicfov[get_config_index()], Vector(m_local->get_vec_eyepos() - center_head).size());

		float normal_fov = g_maths.get_fov(view_angles, angle_to_head);

		if (normal_fov < closest_fov) {
			closest_ent = cur_entity;
			closest_fov = normal_fov;
		}
		
		// prevent symmetry collision
		if (this_fov < g_weebwarecfg.legit_cfg[get_config_index()].maximum_fov && this_fov < best_fov && closest_ent == cur_entity)
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

	matrix3x4 matrix[128];

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

QAngle c_legitbot::calcute_delta(QAngle src, QAngle dst, float f)
{
	QAngle src2 = src;

	QAngle delta = dst - src;

	g_maths.normalize_angle(delta);

	g_maths.clamp_angle(delta);

	delta = src + delta / 100.f * f;

	delta.z = 0;

	return delta;
}

bool c_legitbot::is_visible(c_base_entity* target)
{
	trace_t Trace;

	Vector src = m_local->get_vec_eyepos(), dst2 = target->get_bone(8); // 8 is head. 

	Ray_t ray;

	ray.Init(src, dst2);

	ITraceFilter traceFilter;

	traceFilter.pSkip = (void*)m_local;

	g_weebware.g_engine_trace->TraceRay(ray, MASK_SHOT, &traceFilter, &Trace);

	if (Trace.m_pEnt == target)
		return true;

	if (Trace.fraction == 1.0f)
		return true;

	return false;
}

QAngle c_legitbot::closest_hitbox(c_base_entity* target)
{
	if (!target->is_valid_player())
		return QAngle(0, 0, 0);

	std::vector<int> aim_spots;

	if (g_weebwarecfg.legit_cfg[get_config_index()].hitbox_head)
	{
		aim_spots.push_back(csgohitboxid::head);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].hitbox_chest)
	{
		aim_spots.push_back(csgohitboxid::chest);
	}

	if (g_weebwarecfg.legit_cfg[get_config_index()].hitbox_stomach)
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

QAngle c_legitbot::rcs_scaled(QAngle original_angle)
{
	QAngle delta = original_angle;

	delta.x -= (m_local->m_aimPunchAngle().x * (2.f / 100.f * g_weebwarecfg.legit_cfg[get_config_index()].pitch_rcs));

	delta.y -= (m_local->m_aimPunchAngle().y * (2.f / 100.f * g_weebwarecfg.legit_cfg[get_config_index()].yaw_rcs));

	return delta;
}

#if 0
float GetRealDistanceFOV(float distance, QAngle angle)
{

	Vector aimingAt;
	g_maths::AngleVectors(cmd->viewangles, aimingAt);
	aimingAt *= distance;

	Vector aimAt;
	Math::AngleVectors(angle, aimAt);
	aimAt *= distance;

	return aimingAt.DistTo(aimAt);
}
#endif


void c_legitbot::standalone_rcs(c_usercmd* cmd)
{
	if (!g_weebwarecfg.legit_cfg[get_config_index()].standalone_rcs)
		return;

	static QAngle old_punch = QAngle(0, 0, 0);

	QAngle view_angle = cmd->viewangles;

	QAngle punch = m_local->m_aimPunchAngle() * 2.f;

	QAngle rcs_angle = view_angle - (punch - old_punch);

	QAngle delta = calcute_delta(view_angle, rcs_angle, g_weebwarecfg.legit_cfg[get_config_index()].standalone_rcs_power);

	g_maths.normalize_angle(delta);

	g_maths.clamp_angle(delta);

	if (cmd->buttons & in_attack) {

		cmd->viewangles = delta;

		g_weebware.g_engine->set_view_angles(cmd->viewangles);
	}

	old_punch = punch;
}

long c_legitbot::get_epoch()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

// Althought I dont believe this works hmm

void c_legitbot::c_accuracy_boost::set_abs_origins(c_base_entity* a1, const Vector& a2)
{
	using SetAbsOriginFn = void(__thiscall*)(c_base_entity*, const Vector&);
	static SetAbsOriginFn SetAbsOrigin;

	if (!SetAbsOrigin)
		SetAbsOrigin = (SetAbsOriginFn)(g_weebware.pattern_scan("client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 1F ? ?"));

	SetAbsOrigin(a1, a2);
}

void c_legitbot::c_accuracy_boost::set_abs_angles(c_base_entity* a1, const Vector& a2)
{
	using SetAbsAnglesFn = void(__thiscall*)(c_base_entity*, const Vector&);
	static SetAbsAnglesFn SetAbsAngles;

	if (!SetAbsAngles)
		SetAbsAngles = (SetAbsAnglesFn)(g_weebware.pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8 5D"));

	SetAbsAngles(a1, a2);
}

void c_legitbot::c_accuracy_boost::invalidate_bone_cache(c_base_entity* entity)
{
	static uintptr_t dwBoneCache = g_weebware.pattern_scan("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
	uintptr_t iModelBoneCounter = **(uintptr_t**)(dwBoneCache + 10);
	*(uintptr_t*)((DWORD)entity + 0x2914) = 0xFF7FFFFF;
	*(uintptr_t*)((DWORD)entity + 0x2680) = (iModelBoneCounter - 1);
}


c_legitbot::c_accuracy_boost::c_accuracy_records c_legitbot::c_accuracy_boost::create_record(c_base_entity* entity, c_usercmd* cmd)
{
	c_accuracy_records cur_record;
	// I guess we can store everything, not going to hurt.
	// cur_record.m_angles = entity->GetAbsAngles();
	//cur_record.m_cycle = *entity->m_flCycle();
	//cur_record.m_max = *entity->m_vecMaxs();
	//cur_record.m_mins = *entity->m_vecMins();
	//// cur_record.m_abs_origin = entity->get_abs_origins();
	//cur_record.m_origin = *entity->m_Origin();
	//cur_record.m_sequence = *entity->m_nSequence();
	cur_record.m_simulation_time = *entity->m_flSimulationTime();
	cur_record.m_head = entity->get_bone(8);
	cur_record.record_tick = cmd->tick_count;
	cur_record.index = entity->EntIndex();
#if 1
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
#endif
	return cur_record;
}

void c_legitbot::c_accuracy_boost::set_record(c_base_entity* player, c_accuracy_boost::c_accuracy_records record)
{
	// set_abs_angles(player, record.m_angles);
	// set_abs_origins(player, record.m_abs_origin);
	/**player->m_Origin() = record.m_origin;
	*player->m_flCycle() = record.m_cycle;
	*player->m_vecMaxs() = record.m_max;
	*player->m_vecMins() = record.m_mins;
	*player->m_nSequence() = record.m_sequence;*/
	*player->m_flSimulationTime() = record.m_simulation_time;
}

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

#if 1
bool IsTickValid(float sim)
{
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
#endif
#if 0
bool IsTickValid(float sim_time)
{

	float correct{}, out, in, dt;
	int server_tick;

	in = g_entry.g_engine->get_net_channel()->GetLatency(1);
	out = g_entry.g_engine->get_net_channel()->GetLatency(0);

	correct = out;

	correct = fClamp(correct, 0.f, 1.f);

	server_tick = (g_entry.g_global_vars->tickcount + 1) + TIME_TO_TICKS(in + out);

	dt = correct - (TICKS_TO_TIME(server_tick) - sim_time);

	return std::abs(dt) <= 0.2f;
}
#endif

bool c_legitbot::c_accuracy_boost::is_valid_record(c_accuracy_records record)
{
	if (!IsTickValid(record.m_simulation_time))
	{
		return false;
	}
	return true;
}

void c_legitbot::c_accuracy_boost::clear_all_records()
{
	accuracy_records.clear();
}

void c_legitbot::c_accuracy_boost::accuracy_boost(c_usercmd* cmd)
{
	if (!g_weebwarecfg.legit_cfg[g_legitbot.get_config_index()].accuracy_boost || !g_legitbot.m_local || !g_legitbot.m_local->m_pActiveWeapon() || !g_legitbot.m_local->m_pActiveWeapon()->is_firearm()) {
		clear_all_records();
		return;
	}

	// Create some records...
	for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++)
	{
		c_base_entity * cur_entity = g_weebware.g_entlist->getcliententity(i);

		if (!cur_entity->is_valid_player())
			continue;

		if (cur_entity == g_legitbot.m_local)
			continue;

		if (cur_entity->m_iTeamNum() == g_legitbot.m_local->m_iTeamNum())
			continue;

		if (cur_entity->m_bGunGameImmunity())
			continue;

		accuracy_records.push_back(create_record(cur_entity, cmd));
	}

	float marginal_fov = 180.f;

	for (size_t i = 0; i < accuracy_records.size(); i++)
	{
		bool ExistingPlayer = true;
		auto record_player = g_weebware.g_entlist->getcliententity(accuracy_records[i].index);

		if (!record_player || !record_player->is_valid_player())
			ExistingPlayer = false;

		if (!is_valid_record(accuracy_records[i]) || !ExistingPlayer 
			|| ((cmd->tick_count - g_weebwarecfg.legit_maximum_ticks[g_weebwarecfg.legit_cfg_index]) >= accuracy_records[i].record_tick))
		{
			accuracy_records.erase(accuracy_records.begin() + i);
			continue;
		}


		QAngle angle_to_hitbox;

		g_maths.vector_qangles(accuracy_records[i].m_head - g_legitbot.m_local->get_vec_eyepos(), angle_to_hitbox);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);

		g_weebware.g_engine->get_view_angles(view_angles);

		view_angles += g_legitbot.m_local->m_aimPunchAngle() * 2.f;

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

float random_float(float flMinVal, float flMaxVal)
{
	typedef float(__cdecl * RandomFloatFn)(float, float);
	static RandomFloatFn randomFloat = (RandomFloatFn)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
	return randomFloat(flMinVal, flMaxVal);
}

void random_seed(UINT Seed)
{
	typedef void(*RandomSeed_t)(UINT);
	static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
	m_RandomSeed(Seed);
}

Vector get_spread(c_basecombat_weapon* weapon, int seed)
{
	random_seed((seed & 0xFF) + 1);

	constexpr float pi = PI;
	constexpr float pi2 = pi * 2;

	float a = random_float(0, pi2);
	float flRandomInaccuracy = random_float(0, weapon->Get_Innacuracy());
	float c = random_float(0, pi2);

	float flRandomSpread = random_float(0, weapon->GetSpread());

	float x = (cos(a) * flRandomSpread) + (cos(c) * flRandomInaccuracy);
	float y = (sin(a) * flRandomSpread) + (sin(c) * flRandomInaccuracy);
	return Vector(x, y, 0);
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
void c_legitbot::triggerbot_main(c_usercmd* cmd)
{
	switch (g_weebwarecfg.legit_cfg[g_legitbot.get_config_index()].triggerbot_active) {
	case 0:
		return;
	case 1:
		if (!(GetAsyncKeyState(g_weebwarecfg.legit_cfg[get_config_index()].triggerbot_key)))
			return;

		break;
	}

	//g_legitbot.g_local
	QAngle view_angles = QAngle(0.f, 0.f, 0.f);

	g_weebware.g_engine->get_view_angles(view_angles);

	view_angles += m_local->m_aimPunchAngle() * 2.f;

	Vector forward = Vector(0, 0, 0);

	g_maths.qangle_vector(view_angles, forward);

	// The ray
	forward = m_local->get_vec_eyepos() + (forward * m_local->m_pActiveWeapon()->get_weapon_info()->range);

	c_weapon_info* info = m_local->m_pActiveWeapon()->get_weapon_info();

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

	/*
	these are the hitboxes
	HITGROUP_GENERIC    0
	HITGROUP_HEAD       1
	HITGROUP_CHEST      2
	HITGROUP_STOMACH    3
	HITGROUP_LEFTARM    4
	HITGROUP_RIGHTARM   5
	HITGROUP_LEFTLEG    6
	HITGROUP_RIGHTLEG   7
	HITGROUP_GEAR       10
	*/

	if (g_weebwarecfg.legit_cfg[g_legitbot.get_config_index()].triggerbot_head)
		hitboxes.push_back(1);

	if (g_weebwarecfg.legit_cfg[g_legitbot.get_config_index()].triggerbot_chest)
		hitboxes.push_back(2);

	if (g_weebwarecfg.legit_cfg[g_legitbot.get_config_index()].triggerbot_stomach)
		hitboxes.push_back(3);

	bool has_hitgroup = false;

	for (auto hitbox : hitboxes)
	{
		if (trace.hitgroup == hitbox) {
			has_hitgroup = true;
		}
	}

	static int queue_shot = 0;



	if (has_hitgroup) {
		if (raytrace_hc(view_angles, g_weebwarecfg.legit_cfg[g_legitbot.get_config_index()].triggerbot_hitchance, trace_entity, m_local->m_pActiveWeapon()->get_weapon_info()->range) && next_attack_queued()) {
			cmd->buttons |= in_attack;
		}
		else {
			cmd->buttons &= ~in_attack;
		}
	}


}



