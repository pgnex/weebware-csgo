#include "Header.h"
#include "checksum_md5.hpp"

c_engine_prediction g_engine_prediction;


int corrected_tickbase(c_usercmd* ucmd, c_base_entity* local) {

	static int g_tick = 0;
	static c_usercmd* last_cmd = nullptr;

	if (!ucmd)
		return g_tick;

	if (!last_cmd || last_cmd->hasbeenpredicted) {
		g_tick = local->get_tick_base();
	}
	else {	
		// Required because prediction only runs on frames, not ticks
		// So if your framerate goes below tickrate, m_nTickBase won't update every tick
		++g_tick;
	}

	last_cmd = ucmd;
	return g_tick;
}


void c_engine_prediction::begin(c_usercmd *userCMD, c_base_entity* player)
{
	static bool init = false;

	if (!init)
	{
		predictionRandomSeed = *reinterpret_cast<int**>(g_weebware.pattern_scan("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 0x2);
		predictionPlayer = *reinterpret_cast<c_base_entity**>(g_weebware.pattern_scan("client.dll", "89 3D ? ? ? ? F3 0F 10 47") + 0x2);

		init = true;
	}

	*predictionRandomSeed = MD5_PseudoRandom(userCMD->command_number) & 0x7FFFFFFF;

	predictionPlayer = player;

	old_curtime = g_weebware.g_global_vars->curtime;
	old_frametime = g_weebware.g_global_vars->frametime;
	g_weebware.g_global_vars->curtime = (float)((float)(corrected_tickbase(userCMD, player)) * ((float)g_weebware.g_global_vars->interval_per_tick));
	//g_entry.g_global_vars->curtime = player->get_tick_base() * g_entry.g_global_vars->interval_per_tick;
	g_weebware.g_global_vars->frametime = g_weebware.g_global_vars->interval_per_tick;

	//Here we're doing CBasePlayer::UpdateButtonState // NOTE: hard to tell when offsets changed, think of more longterm solution or just dont do this.
	move_data.m_nButtons = userCMD->buttons;
	int buttonsChanged = userCMD->buttons ^ *reinterpret_cast<int*>(uintptr_t(player) + 0x31E8);
	*reinterpret_cast<int*>(uintptr_t(player) + 0x31DC) = (uintptr_t(player) + 0x31E8);
	*reinterpret_cast<int*>(uintptr_t(player) + 0x31E8) = userCMD->buttons;
	*reinterpret_cast<int*>(uintptr_t(player) + 0x31E0) = userCMD->buttons & buttonsChanged;  //m_afButtonPressed ~ The changed ones still down are "pressed"
	*reinterpret_cast<int*>(uintptr_t(player) + 0x31E4) = buttonsChanged & ~userCMD->buttons; //m_afButtonReleased ~ The ones not down are "released"

	g_weebware.g_game_movement->starttrackpredictionerrors(player);

	memset(&move_data, 0, sizeof(c_move_data));
	g_weebware.g_move_helper->sethost(player);
	g_weebware.g_prediction->setup_move(player, userCMD, g_weebware.g_move_helper, &move_data);
	g_weebware.g_game_movement->processmovement(player, &move_data);
	g_weebware.g_prediction->finish_move(player, userCMD, &move_data);
}

void c_engine_prediction::end(c_base_entity* player)
{
	g_weebware.g_game_movement->finishtrackpredictionerrors(player);
	g_weebware.g_move_helper->sethost(nullptr);

	*predictionRandomSeed = -1;
	predictionPlayer = nullptr;

	g_weebware.g_global_vars->curtime = old_curtime;
	g_weebware.g_global_vars->frametime = old_frametime;
}
