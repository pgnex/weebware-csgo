#include "events.h"
#include <iostream>
#pragma comment(lib, "Winmm.lib")

int attacker, flHurtTime;

void EventFuncs::player_hurt(i_game_event *event) {

	// hitmarkers
	if (g_weebwarecfg.visuals_hitmarkers) {
		// check if you hit a player
		if (g_weebware.g_engine->GetPlayerForUserID(event->GetInt("attacker")) == g_weebware.g_engine->get_local());
		{
			flHurtTime = g_weebware.g_global_vars->curtime + 1.f;
			//	PlaySoundA("C:\\weebware\\sounds\\Hitmarker.wav", NULL, SND_ASYNC);
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

// initialize our events
void GameEvents::init() {

	add_event("player_hurt", EventFuncs::player_hurt);

	register_events();
}