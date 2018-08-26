#include "Header.h"
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
	}

	if (cmd && cmd->command_number)
	{
		Vector original_angles = cmd->viewangles;

		if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		{
			g_create_move.local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

			if (g_create_move.local->is_valid_player())
			{
				try
				{
					g_create_move.local->m_pActiveWeapon()->Update_Accuracy_Penalty();

					g_create_move.create_move(cmd, sendpacket);

#pragma region Legit
					g_legitbot.m_local = g_create_move.local;

					g_legitbot.create_move(cmd);

					g_accuracy.accuracy_boost(cmd);
#pragma endregion

					g_ai.create_move(cmd, g_create_move.local);

#pragma region Clamping
				}
				catch (...)
				{
					printf("Exception Caught in Createmove\n");
				}

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

#pragma endregion
			}
			else
			{
				try {
					g_accuracy.clear_all_records();
				}
				catch (...) {}

				g_Walkbot.m_target_area = nullptr;
			}
		}

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

void c_create_move::create_move(c_usercmd* cmd, bool& sendPackets)
{

	if (!g_weebwarecfg.enable_misc)
		return;

	auto_jump(cmd);

	run_fake(cmd, sendPackets);

	run_legitAA(cmd, sendPackets);
}

void c_create_move::auto_jump(c_usercmd* cmd)
{
	if (!g_weebwarecfg.auto_jump)
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

// I don't really want this autistic shit in here.
#if 0
void reconnect()
{
	static auto fn = reinterpret_cast<unsigned int(__stdcall*)(int, int)>(g_entry.pattern_scan("client.dll", "A1 ? ? ? ? D1 E8 A8 01 74 29"));
	fn(0, 0);
}

char** printisongoingmatchlol()
{
	//55 8B EC A1 ? ? ? ? 8B 4D 08  client // we dont know much about the params. heres the parent function

	static auto fn = reinterpret_cast<char**(*)()>(g_entry.pattern_scan("client.dll", "55 8B EC A1 ? ? ? ? 83 EC 10 A8 01 0F 85 ? ? ? ? C7 45 ? ? ? ? ? 83 C8 01 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? A3 ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 10 45 F0 C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 0F 10 45 F0 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 0F 11 05 ? ? ? ?"));
	return fn();
}
#endif

void c_create_move::run_fake(c_usercmd* cmd, bool &send_packet)
{
	if (!g_weebwarecfg.misc_legit_aa_enabled)
		return;

	send_packet = (cmd->tick_count % 3 == 0);
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
	if (!g_weebwarecfg.misc_legit_aa_enabled)
		return;

	if (local->m_pActiveWeapon()->is_grenade())
		return;

	if (local->GetMoveType() == MOVETYPE_LADDER)
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

			std::cout << diff << std::endl;
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
}