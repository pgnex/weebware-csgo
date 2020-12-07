#include "Header.h"
#include "shared.h"
#include "paint_traverse.h"
#include "esp.h"
#include "hook_funcs.h"
#include "Legit.h"
#include "scene_end.h"
#include "legit_aa.h"

c_paint_traverse g_paint_traverse;

void hooks::hook_functions::paint_traverse( unsigned int v, bool f, bool a ) {

	if ( g_weebwarecfg.misc_ai ) {
		auto ilocal = g_weebware.g_entlist->getcliententity( g_weebware.g_engine->get_local( ) ); // getting localplayer
		if ( !ilocal || ilocal->m_bGunGameImmunity( ) || !ilocal->is_valid_player( ) || !g_weebware.g_engine->is_connected( ) || !g_weebware.g_engine->is_in_game( ) || ilocal->m_fFlags( ) & fl_frozen ) {
			g_Walkbot.m_target_area = nullptr;
			g_Walkbot.m_TargetEntity = nullptr;
		}
	}


	if ( strstr( g_weebware.g_panel->getname( v ), "FocusOverlayPanel" ) ) {

		g_weebware.g_panel->set_mouseinput_enabled( v, g_weebware.menu_opened );

		// if they DONT have screenshot proof on and user is taking a screenshot, render esp
		if ( !( g_weebwarecfg.screenshot_proof && g_weebware.g_engine->is_taking_screenshot( ) ) ) {
			g_esp.esp_main( );
		}
	}
}

void c_paint_traverse::draw_legit_aa_indicator() {

	if (!g_weebwarecfg.legit_aa_indicator)
		return;

	int w, h; g_weebware.g_engine->get_screen_dimensions(w, h);

	wchar_t buf[128];
	int tw, th;

	if (MultiByteToWideChar(CP_UTF8, 0, "<", -1, buf, 128) > 0) {
		g_weebware.g_surface->gettextsize(g_weebware.indicator_font_XD, buf, tw, th);
		draw_string(g_weebware.indicator_font_XD, ((w / 2) - (tw / 2)) - 48, h / 2, (GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? c_color(255, 0, 255, 255) : c_color(255, 255, 255, 255)), 0, "<");
	}

	if (MultiByteToWideChar(CP_UTF8, 0, GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? "Real" : "Fake", -1, buf, 128) > 0) {
		g_weebware.g_surface->gettextsize(g_weebware.tahoma_font_large, buf, tw, th);
		draw_string(g_weebware.tahoma_font_large, ((w / 2) - (tw / 2)) - 48, (h / 2) + 24, (GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? c_color(255, 0, 255, 255) : c_color(255, 255, 255, 255)), 0, GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? "Real" : "Fake");
	}

	if (MultiByteToWideChar(CP_UTF8, 0, ">", -1, buf, 128) > 0) {
		g_weebware.g_surface->gettextsize(g_weebware.indicator_font_XD, buf, tw, th);
		draw_string(g_weebware.indicator_font_XD, ((w / 2) - (tw / 2)) + 48, h / 2, (GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? c_color(255, 255, 255, 255) : c_color(255, 0, 255, 255)), 0, ">");
	}

	if (MultiByteToWideChar(CP_UTF8, 0, GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? "Fake" : "Real", -1, buf, 128) > 0) {
		g_weebware.g_surface->gettextsize(g_weebware.tahoma_font_large, buf, tw, th);
		draw_string(g_weebware.tahoma_font_large, ((w / 2) - (tw / 2)) + 48, (h / 2) + 24, (GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? c_color(255, 255, 255, 255) : c_color(255, 0, 255, 255)), 0, GetKeyState(g_weebwarecfg.misc_legit_aa_side_key) ? "Fake" : "Real");
	}
	
}

void c_paint_traverse::draw_water_mark()
{

	if (g_weebware.g_engine->is_taking_screenshot() && g_weebwarecfg.screenshot_proof) {
		return;
	}

	c_color col = c_color(g_weebwarecfg.water_mark_col);
	std::string weebware = "weebware - " + g_weebware.g_user_name;
	std::transform(weebware.begin(), weebware.end(), weebware.begin(), ::tolower);
	draw_string(g_weebware.tahoma_font, 5, 15, c_color(col.r, col.g, col.b, col.a), 0, weebware.c_str());
}

void c_paint_traverse::draw_string(unsigned long font, int x, int y, c_color color, DWORD alignment, const char* msg, ...) // 1 is right, 0 is normal
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int r = 255, g = 255, b = 255, a = 255;

	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;

	int width, height;

	g_weebware.g_surface->gettextsize(font, wbuf, width, height);

	if (alignment & 1)
		x -= width;
	if (alignment & 2)
		x -= width / 2;

	g_weebware.g_surface->drawsettextfont(font);
	g_weebware.g_surface->drawsettextcolor(r, g, b, a);
	g_weebware.g_surface->drawsettextpos(x, y - height / 2);
	g_weebware.g_surface->drawprinttext(wbuf, wcslen(wbuf));
}