#include "Header.h"
#include "events.h"
#include "shared.h"
#include "create_move.h"
#include "Legit.h"
#include "hook_funcs.h"
#include <intrin.h>

c_create_move g_create_move;

bool hook_functions::clientmode_cm(float input_sample_time, c_usercmd* cmd, bool& sendpacket)
{
	if (cmd->command_number == 0)
	{
		return g_hooking.o_createmove(g_weebware.g_client_mode, input_sample_time, cmd);
		// return PLH::FnCast(g_hooking.cm_tramp, g_hooking.o_createmove)(g_weebware.g_client_mode, input_sample_time, cmd);
	}

	if (cmd && cmd->command_number)
	{
		Vector original_angles = cmd->viewangles;

		if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		{
			g_create_move.local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

			g_create_move.runClanTag();
			g_create_move.chat_spam();
			g_create_move.rank_reveal();

			if (g_create_move.local->is_valid_player() && g_create_move.local->m_pActiveWeapon())
			{
				// shouldn't actually do anything when applying skin cause entites are all refreshed and cause nullptr issues
				if (g_weebwarecfg.skinchanger_apply_nxt) {

					g_accuracy.clear_all_records();

					if (*g_weebware.g_client_state)
						(*g_weebware.g_client_state)->force_update();

					g_accuracy.clear_all_records();

					g_weebwarecfg.skinchanger_apply_nxt = 0;
				}
				else {

					g_create_move.local->m_pActiveWeapon()->Update_Accuracy_Penalty();

					g_legitbot.m_local = g_create_move.local;

					g_legitbot.create_move(cmd);

					g_create_move.create_move(cmd, sendpacket);

					g_accuracy.accuracy_boost(cmd);

					g_ai.create_move(cmd, g_create_move.local);

				}

#pragma region Clamping


				QAngle cmd_view = cmd->viewangles;

				g_maths.normalize_angle(cmd_view);

				g_maths.clamp_angle(cmd_view);

				cmd->viewangles = cmd_view;

				g_create_move.correct_movement(original_angles, cmd);

				g_maths.normalize_angle(cmd->viewangles);

				g_maths.clamp_angle(cmd->viewangles);

				// clamping movement.

				if (cmd->forwardmove > 450)
					cmd->forwardmove = 450;

				if (cmd->forwardmove < -450)
					cmd->forwardmove = -450;

				if (cmd->sidemove > 450)
					cmd->sidemove = 450;

				if (cmd->sidemove < -450)
					cmd->sidemove = -450;
			}
#pragma endregion
		}
		else
		{
			g_accuracy.clear_all_records();
			g_Walkbot.m_target_area = nullptr;
		}
	}
	else
	{
		g_accuracy.clear_all_records();
		g_Walkbot.m_target_area = nullptr;
		g_Walkbot.m_maploaded = false;
	}

	return false;
}

void c_create_move::correct_movement(Vector old_view_angles, c_usercmd* cmd)
{
	Vector o_move(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float speed = o_move.size(), yaw;
	QAngle wish_dir;
	g_maths.vector_qangles3d(o_move, wish_dir);
	yaw = DEG2RAD(cmd->viewangles.y - old_view_angles.y + wish_dir.y);
	cmd->forwardmove = cos(yaw) * speed;
	cmd->sidemove = sin(yaw) * speed;
	if (cmd->viewangles.x < -90.f || cmd->viewangles.x > 90.f)
		cmd->forwardmove = -cmd->forwardmove;
}

namespace anti_trigger {

	/*Credits https://stackoverflow.com/questions/2752725/finding-whether-a-point-lies-inside-a-rectangle-or-not post by Alghyaline*/
	Vector vect2d(Vector p1, Vector p2) {
		Vector temp;
		temp.x = (p2.x - p1.x);
		temp.y = -1 * (p2.y - p1.y);

		return temp;
	}
	bool pointInRectangle(Vector A, Vector B, Vector C, Vector D, Vector m) {
		Vector AB = vect2d(A, B);  float C1 = -1 * (AB.y*A.x + AB.x*A.y); float  D1 = (AB.y*m.x + AB.x*m.y) + C1;
		Vector AD = vect2d(A, D);  float C2 = -1 * (AD.y*A.x + AD.x*A.y); float D2 = (AD.y*m.x + AD.x*m.y) + C2;
		Vector BC = vect2d(B, C);  float C3 = -1 * (BC.y*B.x + BC.x*B.y); float D3 = (BC.y*m.x + BC.x*m.y) + C3;
		Vector CD = vect2d(C, D);  float C4 = -1 * (CD.y*C.x + CD.x*C.y); float D4 = (CD.y*m.x + CD.x*m.y) + C4;
		return     0 >= D1 && 0 >= D4 && 0 <= D2 && 0 >= D3;
	}
	/*credits end stackoverflow*/
	bool Checkifbetween(Vector *ViewanglePoints, Vector Viewangles)
	{
		if (pointInRectangle(ViewanglePoints[0], ViewanglePoints[1], ViewanglePoints[2], ViewanglePoints[3], Viewangles) //first 4 normal
			|| pointInRectangle(ViewanglePoints[3], ViewanglePoints[2], ViewanglePoints[1], ViewanglePoints[0], Viewangles) //first 4 reverse
			|| pointInRectangle(ViewanglePoints[4], ViewanglePoints[5], ViewanglePoints[6], ViewanglePoints[7], Viewangles) //second 4 normal
			|| pointInRectangle(ViewanglePoints[7], ViewanglePoints[6], ViewanglePoints[5], ViewanglePoints[4], Viewangles))//second 4 reverse
		{
			return true;
		}
		return false;
	}

	bool require_fake = 0;

	void run_choke(bool& sendpacket, c_usercmd* cmd)
	{
		if (!require_fake)
			return;

		static int ticks_choked = 0;

		if (ticks_choked > 17) {
			sendpacket = true;
			ticks_choked = 0;
			require_fake = 0;
		}
		else {
			sendpacket = false;
		}

		ticks_choked++;
	}
}

void c_create_move::rank_reveal()
{
	if (!g_weebwarecfg.rank_reveal) return;

	using ServerRankRevealAll = char(__cdecl*)(int*);

	static uint8_t* fnServerRankRevealAll = 0;

	if (!fnServerRankRevealAll) {
			fnServerRankRevealAll = (uint8_t*)g_weebware.pattern_scan(("client_panorama.dll"), "55 8B EC 8B 0D ? ? ? ? 85 C9 75 ? A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 ? 85 C0 74 ? 8B C8 E8 ? ? ? ? 8B C8 EB ? 33 C9 89 0D ? ? ? ? 8B 45 ? FF 70 ? E8 ? ? ? ? B0 ? 5D");
	}

	int v[3] = { 0,0,0 };

	reinterpret_cast<ServerRankRevealAll>(fnServerRankRevealAll)(v);
}

void c_create_move::create_move(c_usercmd* cmd, bool& sendPackets)
{
	if (!g_weebwarecfg.enable_misc)
		return;

	auto_jump(cmd);

	run_fake(cmd, sendPackets);

	if (g_weebwarecfg.misc_legit_aa_enabled && !anti_trigger::require_fake)
		run_legitAA(cmd, sendPackets);


}

void c_create_move::auto_jump(c_usercmd* cmd)
{
	if (!g_weebwarecfg.auto_jump)
	{
		return;
	}

	if (local->get_move_type() == MOVETYPE_LADDER)
	{
		return;
	}

	// If Local is in air.
	if (!(this->local->m_fFlags() & fl_onground))
	{
		// release jump flag.
		cmd->buttons &= ~in_jump;
	}
}

bool c_create_move::is_visible(c_base_entity* target)
{
	trace_t Trace;

	Vector src = local->get_vec_eyepos(), dst2 = target->get_bone(8); // 8 is head. 

	Ray_t ray;

	ray.Init(src, dst2);

	ITraceFilter traceFilter;

	traceFilter.pSkip = (void*)local;

	g_weebware.g_engine_trace->TraceRay(ray, MASK_SHOT, &traceFilter, &Trace);

	if (Trace.m_pEnt == target)
		return true;

	if (Trace.fraction == 1.0f)
		return true;

	return false;
}


// https://www.unknowncheats.me/forum/counterstrike-global-offensive/258333-antitrigger.html
// thanks master looser for these box tracing functions <3

bool c_create_move::anti_trigger(c_usercmd* cmd, bool& send_packets)
{
	switch (g_weebwarecfg.anti_triggerbot) {
	case 0:
		return false;

	case 2:
		if (!GetAsyncKeyState(g_weebwarecfg.anti_triggerbot_key))
			return false;
	}

	auto org = *local->m_Origin();

	org += (local->m_vecVelocity() / 6);

	auto min = *local->m_vecMins();
	auto max = *local->m_vecMaxs();

	Vector Local_Bounding[8] = { //the comments below are not accurate
		Vector(org.x + min.x, org.y + min.y, org.z + min.z), // left buttom
		Vector(org.x + min.x, org.y + min.y, org.z + max.z), // left top
		Vector(org.x + max.x, org.y + max.y, org.z + max.z), // right top
		Vector(org.x + max.x, org.y + max.y, org.z + min.z),  // right buttom

		Vector(org.x + min.x, org.y + max.y, org.z + min.z), // left buttom
		Vector(org.x + min.x, org.y + max.y, org.z + max.z), // left top
		Vector(org.x + max.x, org.y + min.y, org.z + max.z), // right top
		Vector(org.x + max.x, org.y + min.y, org.z + min.z)  // right buttom
	};

	for (auto i = 1; i < 99; i++) {

		c_base_entity* cur_entity = (c_base_entity*)g_weebware.g_entlist->getcliententity(i);

		if (!cur_entity)
			continue;

		if (!cur_entity->m_iHealth() > 0)
			continue;

		if (cur_entity->m_iTeamNum() == local->m_iTeamNum())
			continue;

		QAngle viewangle = *cur_entity->eyeangle_ptr();

		Vector direction;

		g_maths.qangle_vector(viewangle, direction);
		direction *= 8012.f;

		Vector begin = cur_entity->get_vec_eyepos();
		Vector finish = begin + direction;

		Vector angles[8];

		for (auto i = 0; i < 8; i++)
			g_maths.vector_qangles(Local_Bounding[i] - begin, angles[i]);

		if (!anti_trigger::Checkifbetween(angles, viewangle))
			continue;

		trace_t trace;
		Ray_t ray;
		ITraceFilter filter;
		filter.pSkip = cur_entity;
		ray.Init(begin, finish);
		g_weebware.g_engine_trace->TraceRay(ray, 0x46004003, &filter, &trace);

		if (((trace.endpos - begin).size() + 10) >= (local->get_vec_eyepos() - begin).size()) {

			anti_trigger::require_fake = true;

			break;
		}
	}

	return anti_trigger::require_fake;
}

void c_create_move::run_fake(c_usercmd* cmd, bool &send_packet)
{
	anti_trigger::run_choke(send_packet, cmd);

	if (anti_trigger(cmd, send_packet))
		return;

	if (!g_weebwarecfg.misc_legit_aa_enabled)
		return;

	send_packet = (cmd->tick_count % 4 == 0);
}

bool can_shoot(c_base_entity* local)
{
	if (local->m_pActiveWeapon()->m_flNextPrimaryAttack() > (float)g_weebware.g_global_vars->curtime)
		return false;
	return true;
}

c_base_entity* get_best_target(c_base_entity * local)
{
	float best_fov = 360.f;

	c_base_entity* best_entity = nullptr;

	for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++)
	{
		c_base_entity* cur_entity = (c_base_entity*)g_weebware.g_entlist->getcliententity(i);

		if (!cur_entity)
			continue;

		if (!cur_entity->m_iHealth() > 0)
			continue;

		if (cur_entity->m_iTeamNum() == local->m_iTeamNum())
			continue;

		Vector center_head = cur_entity->get_bone(8);

		QAngle angle_to_head;

		g_maths.vector_qangles(center_head - local->get_vec_eyepos(), angle_to_head);

		QAngle view_angles = QAngle(0.f, 0.f, 0.f);

		g_weebware.g_engine->get_view_angles(view_angles);

		view_angles += local->m_aimPunchAngle() * 2.f;

		float this_fov = g_maths.get_fov(view_angles, angle_to_head);

		if (this_fov < best_fov)
		{
			best_entity = cur_entity;
			best_fov = this_fov;
		}
	}
	return best_entity;
}


void c_create_move::run_legitAA(c_usercmd* cmd, bool send_packets)
{
	if (local->m_pActiveWeapon()->is_grenade())
		return;

	if (local->get_move_type() == MOVETYPE_LADDER)
		return;

	if (!(cmd->buttons & in_attack) && !send_packets) {

		//	auto target = get_closest_target_available(this->local);

		c_base_entity* target = get_best_target(this->local);

		QAngle OriginalAngle = cmd->viewangles;

		QAngle angle2Target = cmd->viewangles;

		bool shouldflip = 0;

		if (target) {

			auto hitbox = *target->m_Origin();

			g_maths.vector_qangles(hitbox - this->local->get_vec_eyepos(), angle2Target);

			g_maths.normalize_angle(angle2Target);

			g_maths.normalize_angle(OriginalAngle);

			float diff = (angle2Target.y - OriginalAngle.y);

			// If behind
			if (diff > 0 && diff < 180) {
				shouldflip = 1;
			}
		}

		float reversed = shouldflip ? -90 : 90;

		static int stage = -90;

		stage += 10;

		if (!g_weebwarecfg.misc_legit_aa_jitter)
			cmd->viewangles.y = (angle2Target.y - reversed);
		else
			cmd->viewangles.y = (angle2Target.y - reversed - stage);

		if (stage > 90)
			stage = -90;

	}

	//if (!g_weebwarecfg.misc_slidewalk) {
	//	if (cmd->forwardmove > 0)
	//	{
	//		cmd->buttons |= in_back;
	//		cmd->buttons &= in_forward;
	//	}

	//	if (cmd->forwardmove < 0)
	//	{
	//		cmd->buttons |= in_forward;
	//		cmd->buttons &= in_back;
	//	}

	//	if (cmd->sidemove < 0)
	//	{
	//		cmd->buttons |= in_moveright;
	//		cmd->buttons &= in_moveleft;
	//	}

	//	if (cmd->sidemove > 0)
	//	{
	//		cmd->buttons |= in_moveleft;
	//		cmd->buttons &= ~in_moveright;
	//	}
	//}
}

void c_create_move::chat_spam()
{
	static const std::vector<std::string> cspam_weebware = {
		"weebware.net - premium cheating software, get weebware!",
		"weebware.net - cheating is for cool kids, get weebware!",
		"weebware.net - same price as an esea sub, lol, get weebware!",
		"weebware.net - would u like some sauce with ur pasta?",
		"weebware.net - no sir, i've never heard of ayyware b4",
		"weebware.net - you pay for that baim?",
		"weebware.net - fanta.trashed on",
		"weebware.net - does it come with junkcode",
		"weebware.net - handin out L's",
		"weebware.net - where your security matters."
	};

	if (g_weebwarecfg.misc_chat_spammer)
	{
		static int elapsed_ticks = 0;

		int selection = int(static_cast<int>(cspam_weebware.size()) * rand() / (RAND_MAX + 1.0));

		elapsed_ticks++;

		// calculate server tickrate
		int tick_rate = 1 / g_weebware.g_global_vars->interval_per_tick;

		// after server refresh we should be able to execute another cmd
		if (elapsed_ticks >= tick_rate) {
			g_weebware.g_engine->execute_client_cmd(("say " + cspam_weebware.at(selection)).c_str());
			elapsed_ticks = 0;
		}
	}
}

void c_create_move::runClanTag()
{
	static const std::string MovingTag[16] = { "w", "we", "wee", "weeb", "weebw", "weebwa", "weebwar", "weebware", "weebware", "weebwar", "weebwa", "weebw", "weeb", "wee", "we", "w" };

	if (g_weebwarecfg.misc_clantag_changer && g_weebwarecfg.enable_misc)
	{
		static int ticks_elapsed = 0;

		static auto set_clantag = (int(__fastcall*)(const char*, const char*))((uintptr_t)GetModuleHandleA("engine.dll") + 0x886E0); // g_weebware.pattern_scan("engine.dll", "53 56 57 8B DA 8B F9 FF 15");

		int i = (int(g_weebware.g_global_vars->curtime * 2.4) % 16);

		int tick_rate = 1 / g_weebware.g_global_vars->interval_per_tick;

		if (ticks_elapsed > tick_rate)
		{
			set_clantag(MovingTag[i].c_str(), MovingTag[i].c_str());
			ticks_elapsed = 0;
		}

		ticks_elapsed++;
	}
}