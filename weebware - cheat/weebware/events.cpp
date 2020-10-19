#include "Header.h"
#include "events.h"
#include "frame_stage.h"
#include "create_move.h"
#include "shared_funcs.h"
#include <iostream>
#pragma comment(lib, "Winmm.lib")

int flHurtTime = 0;

FeatureFuncs g_events_features;

void EventFuncs::bullet_impact(i_game_event *event) {
	static int          old_tickbase{ -1 };
	static bool         insert_once{ false };
	static ImpactData_t *entry{ nullptr };

	c_base_entity *player;
	int          tickbase;
	float        curtime;
	Vector       impact_pos;

	int local_player_index = g_weebware.g_engine->get_local();
	c_base_entity* local_player = g_weebware.g_entlist->getcliententity(local_player_index);

	// invalid local player data, don't do anything more.
	if (!local_player)
		return;

	if (!local_player->is_valid_player())
		return;

	// filter out everyone except the local player.
	player = (c_base_entity *)g_weebware.g_entlist->getcliententity(g_weebware.g_engine->GetPlayerForUserID(event->GetInt("userid")));
	if (!player)
		return;

	if (player != local_player)
		return;

	// get main impact vector.
	auto vis_impact_data_main = &vis_impact_data;

	// get the localplayer's tickbase.
	tickbase = player->get_tick_base();

	// tickbase changed, this means new impact(s) happened.
	// note; the server does impact event code during penetration... this means you will receive multiple impacts with the same time.
	if (tickbase != old_tickbase) {

		// allocate new empty entry...
		vis_impact_data_main->push_back(ImpactData_t());

		// now get the back.
		entry = &vis_impact_data_main->back();

		// reset other data.
		insert_once = false;

		// mark the last tickbase.
		old_tickbase = tickbase;
	}

	// this should never happen...
	if (!entry)
		return;

	// get the time this impact happened.
	curtime = (float)tickbase * g_weebware.g_global_vars->interval_per_tick;

	// get impact position.
	impact_pos = Vector(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

	// only store this data once.
	if (!insert_once) {
		entry->m_shoot_pos = local_player->get_vec_eyepos();
		entry->m_tickbase = tickbase;
		entry->m_curtime = curtime;

		insert_once = true;
	}

	// new impact happened... unmark old impacts as the final impact.
	for (auto &i : entry->m_impacts)
		i.m_is_last_impact = false;

	// insert this impact... mark it as the 'final'.
	entry->m_impacts.push_back(ImpactEntry_t(impact_pos, true));
}

void EventFuncs::player_hurt(i_game_event *event) {
	// hitmarkers
	if (g_weebwarecfg.visuals_hitmarkers) {
		// check if you hit a player
		int attacker = g_weebware.g_engine->GetPlayerForUserID(event->GetInt("attacker"));
		c_base_entity* attacker_ent = (c_base_entity *)g_weebware.g_entlist->getcliententity(attacker);
		if (attacker_ent == g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local()))
		{			
			flHurtTime = g_weebware.g_global_vars->curtime + 1.f;
			if (g_weebwarecfg.hitmarker_sound > 0) {
				switch (g_weebwarecfg.hitmarker_sound)
				{
				case 1:
					PlaySoundA((g_weebware.g_appdata_path + "\\sounds\\Hitmarker.wav").c_str(), NULL, SND_ASYNC);
					break;
				case 2:
					PlaySoundA((g_weebware.g_appdata_path + "\\sounds\\anime.wav").c_str(), NULL, SND_ASYNC);
					break;
				case 3:
					PlaySoundA((g_weebware.g_appdata_path + "\\sounds\\bubble.wav").c_str(), NULL, SND_ASYNC);
					break;
				case 4:
					PlaySoundA((g_weebware.g_appdata_path + "\\sounds\\custom.wav").c_str(), NULL, SND_ASYNC);
					break;
				}
			}
		}
	}
}

// draw hitmarker
void FeatureFuncs::on_paint()
{
	if (flHurtTime < g_weebware.g_global_vars->curtime)
		return;

	if (!g_weebware.g_engine->is_connected() || !g_weebware.g_engine->is_in_game())
		return;

	float delta = flHurtTime - g_weebware.g_global_vars->curtime;

	c_color col = g_weebwarecfg.visuals_hitmarker_col;

	g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, (int)(float)(delta * 255.f));

	int x, y;

	g_weebware.g_engine->get_screen_dimensions(x, y);

	x /= 2; y /= 2;

	g_weebware.g_surface->drawline(x - 8, y - 8, x - (8 / 4), y - (8 / 4));
	g_weebware.g_surface->drawline(x - 8, y + 8, x - (8 / 4), y + (8 / 4));
	g_weebware.g_surface->drawline(x + 8, y + 8, x + (8 / 4), y + (8 / 4));
	g_weebware.g_surface->drawline(x + 8, y - 8, x + (8 / 4), y - (8 / 4));
};

void FeatureFuncs::killsay(i_game_event* event) {
	int attacker = g_weebware.g_engine->GetPlayerForUserID(event->GetInt("attacker"));
	c_base_entity* attacker_ent = (c_base_entity*)g_weebware.g_entlist->getcliententity(attacker);

	if (attacker_ent == g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local())) {
		if (g_weebwarecfg.killsay_msg_custom.length() > 0) {
			g_weebwarecfg.killsay_msg_custom = g_weebwarecfg.killsay_msg_custom_buf;
			g_weebware.g_engine->execute_client_cmd(("say " + g_weebwarecfg.killsay_msg_custom).c_str());
		}
		else {
			int selection = int(killsay_messages.size()) * (rand() / (RAND_MAX + 1.0));
			g_weebware.g_engine->execute_client_cmd(("say " + killsay_messages.at(selection)).c_str());
		}

	}
}

void FeatureFuncs::buy_bot() {

	if (g_weebwarecfg.buy_bot_armor) {
		g_weebware.g_engine->execute_client_cmd("buy vest");
	}

	if (g_weebwarecfg.buy_bot_primary > 0) {
		g_weebware.g_engine->execute_client_cmd(prim_buy_bot_map[g_weebwarecfg.buy_bot_primary].c_str());
	}

	if (g_weebwarecfg.buy_bot_secondary > 0) {
		g_weebware.g_engine->execute_client_cmd(sec_buy_bot_map[g_weebwarecfg.buy_bot_secondary].c_str());
	}
}


void EventFuncs::player_death(i_game_event* event) {

	if (g_weebwarecfg.killsay)
		g_events_features.killsay(event);

	// check if you hit a player
	c_base_entity* attacker_ent = (c_base_entity*)g_weebware.g_entlist->getcliententity(g_weebware.g_engine->GetPlayerForUserID(event->GetInt("attacker")));
	c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
	if (attacker_ent == local) {
		if (g_weebwarecfg.killsound)
			PlaySoundA("C:\\weebware\\sounds\\killsound_1.wav", NULL, SND_ASYNC);
	}
}

void EventFuncs::round_end(i_game_event* event) {
	g_weebware.round_end = true;
}

void EventFuncs::game_end(i_game_event* event) {
	flHurtTime = 0;
}

void EventFuncs::round_start(i_game_event* event) {

	if (!g_weebware.g_engine->is_connected() || !g_weebware.g_engine->is_in_game())
		return;

	g_weebware.round_end = false;
	if (g_weebwarecfg.visuals_nightmode) g_nightmode.done = false;
	if (g_weebwarecfg.draw_grenade_traj) g_create_move.grenade_traj_disabled = false;
	if (g_weebwarecfg.night_sky) g_create_move.is_sky_set = false;
	if (g_weebwarecfg.misc_clantag_changer) g_frame_stage_notify.clantag_done = false;

	if (g_weebwarecfg.buy_bot_enabled)
		g_events_features.buy_bot();

	flHurtTime = 0;
}

void EventFuncs::player_spawned(i_game_event* event) {
	if (g_weebwarecfg.buy_bot_enabled)
		if (g_weebware.g_engine->GetPlayerForUserID(event->GetInt("userid")) == g_weebware.g_engine->get_local())
			g_events_features.buy_bot();
}


// initialize our events
void GameEvents::init() {

	add_event("player_hurt", EventFuncs::player_hurt);
	add_event("bullet_impact", EventFuncs::bullet_impact);
	add_event("player_death", EventFuncs::player_death);
	add_event("round_start", EventFuncs::round_start);
	add_event("player_spawned", EventFuncs::player_spawned);
//	add_event("round_end", EventFuncs::round_end);

	register_events();
}