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

}


void c_d3dxesp::water_mark() {

	if (!g_weebwarecfg.visuals_watermark)
		return;

	c_color col = c_color(g_weebwarecfg.water_mark_col);
	std::string watermark_text = ("weebware - " + g_weebware.g_user_name);
	std::transform(watermark_text.begin(), watermark_text.end(), watermark_text.begin(), ::tolower);

	draw.Text(watermark_text.c_str(), 5, 5, lefted, 0, col.r, col.g, col.b, col.a);
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

	if (g_maths.world_to_screen(end, end_screen)) {
		draw.Line(end_screen.x - 10, end_screen.y, end_screen.x + 10, end_screen.y, 2, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
		draw.Line(end_screen.x, end_screen.y - 10, end_screen.x, end_screen.y + 10, 2, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
	}
}

void c_d3dxesp::d3dx_reset() {
	draw.FontReset();
	draw.Reset();
}