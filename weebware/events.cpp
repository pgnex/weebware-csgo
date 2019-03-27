#include "events.h"
#include <iostream>
#pragma comment(lib, "Winmm.lib")

int flHurtTime;

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
	"sit dog",
	"refund",
	"would you like some sauce with that pasta",
	"get fucked nigger",
	"aim next time",
	"can you atleast try to shoot back",
	"wyd my heads up here"
};

void EventFuncs::player_death(i_game_event *event) {

	if (!g_weebwarecfg.killsay)
		return;

	int attacker = g_weebware.g_engine->GetPlayerForUserID(event->GetInt("attacker"));
	c_base_entity* attacker_ent = (c_base_entity *)g_weebware.g_entlist->getcliententity(attacker);

	if (attacker_ent == g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local())) {
		int selection = int(static_cast<int>(killsay_messaes.size()) * rand() / (RAND_MAX + 1.0));
		g_weebware.g_engine->execute_client_cmd(("say " + killsay_messaes.at(selection)).c_str());
	}
}

// initialize our events
void GameEvents::init() {

	add_event("player_hurt", EventFuncs::player_hurt);
	add_event("player_death", EventFuncs::player_death);

	register_events();
}