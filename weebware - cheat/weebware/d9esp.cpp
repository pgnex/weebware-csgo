#include "d9esp.h"
#include <d3d9types.h>

void c_d9esp::d9esp_main(IDirect3DDevice9* pDevice) {
	static bool init_fonts = false;

	draw.GetDevice(pDevice);
	draw.Reset();

	if (!init_fonts) {
		draw.AddFont("Tahoma", 11, false, false);

		init_fonts = true;
	}
	
	if (draw.Font()) draw.OnLostDevice();

	// draw anything before any checks..
	water_mark();

	local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!g_weebware.g_engine->is_connected())
		return;

	if (!g_weebware.g_engine->is_in_game())
		return;

	// move check to main call later
	if (g_weebware.g_engine->is_taking_screenshot() && g_weebwarecfg.screenshot_proof)
		return;

	if (!local)
		return;

	// hitmarker drawings
	g_event_features.on_paint();

	// visuals not needed in ent loop
	draw_inaccuracy_circle();


}


void c_d9esp::water_mark() {

	if (!g_weebwarecfg.visuals_watermark)
		return;

	c_color col = c_color(g_weebwarecfg.water_mark_col);
	std::string watermark_text = ("weebware - " + g_weebware.g_user_name);
	std::transform(watermark_text.begin(), watermark_text.end(), watermark_text.begin(), ::tolower);

	draw.Text(watermark_text.c_str(), 5, 5, lefted, 0, col.r, col.g, col.b, col.a);
}

void c_d9esp::draw_inaccuracy_circle() {

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

void c_d9esp::draw_fov_circle() {

	if (!g_weebwarecfg.visuals_fov_circle)
		return;

	if (!local)
		return;

	if (local->m_iHealth() <= 0)
		return;

	int x, y;
	float fov;

	fov = g_weebwarecfg.legit_cfg[g_legitbot.get_config_index()].maximum_fov;
	g_weebware.g_engine->get_screen_dimensions(x, y);
	c_color col = c_color(g_weebwarecfg.visuals_fov_circle_col);

	float radius = tanf(DEG2RAD(fov) / 2) / tanf(DEG2RAD(g_hooking.o_vm()) / 2) * x;

	g_weebware.g_surface->drawcoloredcircle(x / 2, y / 2, radius / 2, col.r, col.g, col.b, col.a);
}
