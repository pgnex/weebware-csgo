#include "events.h"
#include "frame_stage.h"
#include "create_move.h"
#include <iostream>
#pragma comment(lib, "Winmm.lib")

int flHurtTime;

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
					PlaySoundA("C:\\weebware\\sounds\\Hitmarker.wav", NULL, SND_ASYNC);
					break;
				case 2:
					PlaySoundA("C:\\weebware\\sounds\\anime.wav", NULL, SND_ASYNC);
					break;
				case 3:
					PlaySoundA("C:\\weebware\\sounds\\bubble.wav", NULL, SND_ASYNC);
					break;
				case 4:
					PlaySoundA("C:\\weebware\\sounds\\custom.wav", NULL, SND_ASYNC);
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

static const std::vector<std::string> killsay_messaes = {
	"Supa kawaii desu!!!!!!!! ___________________^",
	"SUPA SUPA SUPA KAWAII SASUKE-SAMA!!!!!",
	"Nyaaaaa!!! (________<) _________________;;;;;;;;;;;;;;;;",
	"I absolutely luuuv @_____@ anime <3",
	"What the desu did you just kawaii say about me, you little baka",
	"wigglez booty n' squirms",
	"please punish me licks lips nyea~",
	"notices buldge OwO what's that",
	"that's a penis UwU you towd me you wewe a giww!!",
	"Cummy desu, you are my senpai",
	"uguu desu desu kawaii neee~~~~~ <3",
	"wiww shit fuwwies aww ovew you and you wiww dwown in them",
	"AYAYA!~ >__<",
	"Nani the fuck did omae just fucking iimasu about watashi, you chiisai bitch desuka?",
	"Uhm by the way MOM They're not Chinese cartoons it's called ANIME!!"
};

void killsay(i_game_event* event) {
	int attacker = g_weebware.g_engine->GetPlayerForUserID(event->GetInt("attacker"));
	c_base_entity* attacker_ent = (c_base_entity*)g_weebware.g_entlist->getcliententity(attacker);

	if (attacker_ent == g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local())) {
		if (strlen(g_weebwarecfg.killsay_msg_custom) > 0) {
			std::string msg = g_weebwarecfg.killsay_msg_custom;
			g_weebware.g_engine->execute_client_cmd(("say " + msg).c_str());
		}
		else {
			int selection = int(static_cast<int>(killsay_messaes.size())* rand() / (RAND_MAX + 1.0));
			g_weebware.g_engine->execute_client_cmd(("say " + killsay_messaes.at(selection)).c_str());
		}

	}
}



void EventFuncs::player_death(i_game_event* event) {

	if (g_weebwarecfg.killsay)
		killsay(event);
}

void EventFuncs::round_end(i_game_event* event) {
	g_weebware.round_end = true;
}

void EventFuncs::round_start(i_game_event* event) {

	if (!g_weebware.g_engine->is_connected() || !g_weebware.g_engine->is_in_game())
		return;

	g_weebware.round_end = false;
	if (g_weebwarecfg.visuals_nightmode) g_nightmode.done = false;
	if (g_weebwarecfg.draw_grenade_traj) g_create_move.grenade_traj_disabled = false;
	if (g_weebwarecfg.night_sky) g_create_move.is_sky_set = false;
	if (g_weebwarecfg.misc_clantag_changer) g_frame_stage_notify.clantag_done = false;
	flHurtTime = 0;
}

// initialize our events
void GameEvents::init() {

	add_event("player_hurt", EventFuncs::player_hurt);
	add_event("bullet_impact", EventFuncs::bullet_impact);
	add_event("player_death", EventFuncs::player_death);
	add_event("round_end", EventFuncs::round_end);
	add_event("round_start", EventFuncs::round_start);

	register_events();
}