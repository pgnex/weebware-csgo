#include "drawing.h"
#include "d3dx9esp.h"
#include "Legit.h"
#include <d3d9types.h>

c_draw draw;

void c_d3dxesp::d9esp_main(IDirect3DDevice9* pDevice) {
	static bool init = false;

	draw.GetDevice(pDevice);

	if (!init) {
		draw.AddFont("Tahoma", 11, false, false);
		draw.AddFont("Tahoma", 18, false, false);
		draw.Reset();
		init = true;
	}
	
	if (draw.Font()) draw.OnLostDevice();

	// draw anything before any checks..
	water_mark();


	if (!g_weebware.g_engine->is_connected())
		return;

	if (!g_weebware.g_engine->is_in_game())
		return;

	local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!local)
		return;

	// hitmarker drawings
	//g_events.on_paint();

	// visuals not needed in ent loop
	draw_inaccuracy_circle();
	draw_sniper_crosshair();
	recoil_crosshair();


	for (int i = 1; i <= g_weebware.g_entlist->getmaxentities(); i++)
	{
		c_base_entity* ent = g_weebware.g_entlist->getcliententity(i);

		if (!ent || ent == nullptr)
			continue;

		// check if entity is bomb
		if (strstr(ent->get_client_class()->m_networkedname, "CPlantedC4")) {
			bomb_timer(ent);
		}


		if (!ent || ent->m_iHealth() <= 0 || ent->get_client_class()->m_ClassID != class_ids::ccsplayer) {
			continue;
		}

		//if (g_weebwarecfg.anime_model > 0)
		//	set_player_models(ent);
	}

}


void c_d3dxesp::water_mark() {

	if (!g_weebwarecfg.visuals_watermark)
		return;

	c_color col = c_color(g_weebwarecfg.water_mark_col);
	std::string watermark_text = ("weebware - " + g_weebware.g_user_name);
	std::transform(watermark_text.begin(), watermark_text.end(), watermark_text.begin(), ::tolower);

	draw.Text(watermark_text.c_str(), 5, 5, lefted, tahoma, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
}

void c_d3dxesp::draw_inaccuracy_circle() {

	if (!g_weebwarecfg.visuals_inacc_circle)
		return;

	auto weapon = local->m_pActiveWeapon();

	if (!weapon)
		return;

	if (!weapon->is_firearm())
		return;

	c_color col = c_color(g_weebwarecfg.visuals_innacc_circle_col);

	draw.Circle(draw.Screen.x_center, draw.Screen.y_center, weapon->Get_Innacuracy() * 200, 0, full, true, 24, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
}

void c_d3dxesp::draw_sniper_crosshair() {

	if (!g_weebwarecfg.visuals_sniper_crosshair)
		return;

	c_basecombat_weapon* weapon = local->m_pActiveWeapon();

	if (!weapon || weapon == nullptr)
		return;

	if (!weapon->is_firearm())
		return;

	if (!weapon->is_autosniper() && !weapon->is_awp() && !weapon->is_scout())
		return;


	c_color col = g_weebwarecfg.visuals_sniper_crosshair_col;
	int cx = draw.Screen.x_center;
	int cy = draw.Screen.y_center;
	draw.Line(cx - 8, cy - 0, cx + 8, cy + 0, 2, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
	draw.Line(cx + 0, cy - 8, cx - 0, cy + 8, 2, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
}

void c_d3dxesp::recoil_crosshair() {

	if (!g_weebwarecfg.visuals_recoil_crosshair)
		return;

	Vector view_angles;
	g_weebware.g_engine->get_view_angles(view_angles);

	view_angles += local->m_aimPunchAngle() * 2;

	Vector forward_vec;
	g_maths.qangle_vector(view_angles, forward_vec);
	forward_vec *= 10000;

	Vector start = local->get_vec_eyepos();
	Vector end = start + forward_vec, end_screen;

	c_color col = g_weebwarecfg.visuals_recoil_crosshair_col;

	if (g_maths.CRWorldToScreen(end, end_screen)) {
		draw.Line(end_screen.x - 10, end_screen.y, end_screen.x + 10, end_screen.y, 2, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
		draw.Line(end_screen.x, end_screen.y - 10, end_screen.x, end_screen.y + 10, 2, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
	}
}

void c_d3dxesp::bomb_timer(c_base_entity* ent) {

	if (!g_weebwarecfg.visuals_bomb_timer)
		return;

	float remaining = reinterpret_cast<c_bomb*>(ent)->get_blow_time() - g_weebware.g_global_vars->curtime;

	// So c4 is still going!
	if (reinterpret_cast<c_bomb*>(ent)->is_ticking() && remaining > 0.f) {

		int offset_y = 15.f;

		if (g_weebwarecfg.visuals_watermark) {
			offset_y += 35.f;
		}

		int Damage = 500;
		float BombRadius = 1750;

		auto vecToTarget = ent->get_vec_eyepos() - local->get_vec_eyepos();

		// + 1 to account for imprecision, also magic number btw.

		float predicted_damage = 500.f * exp(-((vecToTarget.size() * vecToTarget.size()) / 680555.582778f));

		if ((local->m_ArmorValue() > 0)) {

			predicted_damage *= 0.5f;

			if ((predicted_damage / 2) > local->m_ArmorValue())
			{
				predicted_damage += (local->m_ArmorValue() * 2);
			}
		}
		predicted_damage += 1.f;

		predicted_damage = ceilf(predicted_damage);


		Vector bomb_pos = ent->getrenderorigin();
		Vector screen_pos;

		if (!g_maths.CRWorldToScreen(bomb_pos, screen_pos))
			return;

		char time_text[55];
		char damage_text[55];

		sprintf(damage_text, "Damage: %.0f", floor(predicted_damage));
		sprintf(time_text, "Time: %.2fs", remaining);

		int tw1, tw2;

		tw1 = draw.GetTextWidth(time_text, draw.pFont[tahoma_large]);
		tw2 = draw.GetTextWidth(damage_text, draw.pFont[tahoma_large]);


		draw.Text(time_text, screen_pos.x - (tw1 / 2), screen_pos.y - 40, lefted, tahoma_large, predicted_damage < local->m_iHealth() ? D3DCOLOR_ARGB(255, 0, 255, 0) : D3DCOLOR_ARGB(255, 255, 0, 0));
		draw.Text(damage_text, screen_pos.x - (tw2 / 2), screen_pos.y - 25, lefted, tahoma_large, predicted_damage < local->m_iHealth() ? D3DCOLOR_ARGB(255, 0, 255, 0) : D3DCOLOR_ARGB(255, 255, 0, 0));
	}
}

void c_d3dxesp::d3dx_reset() {
	draw.FontReset();
	draw.Reset();
}