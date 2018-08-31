#include "Header.h"
#include "shared.h"
#include "paint_traverse.h"
#include "esp.h"
#include "hook_funcs.h"
#include "Legit.h"

c_paint_traverse g_paint_traverse;

void hook_functions::paint_traverse(unsigned int v, bool f, bool a)
{
	// PLH::FnCast(g_hooking.paint_tramp, g_hooking.o_painttraverse)(g_weebware.g_panel, v, f, a);
	g_hooking.o_painttraverse(g_weebware.g_panel, v, f, a);

	try {
		if (!g_weebware.g_engine->is_in_game() || !g_weebware.g_engine->is_connected())
			g_accuracy.clear_all_records();

		if (strstr(g_weebware.g_panel->getname(v), "FocusOverlayPanel")) {

			g_weebware.g_panel->set_mouseinput_enabled(v, g_weebware.menu_opened);

			g_esp.esp_main();
		}
	}
	catch(...) {}
}

void c_paint_traverse::draw_water_mark()
{
	c_color col = c_color(g_weebwarecfg.water_mark_col);
	draw_string(g_weebware.tahoma_font, 5, 20, c_color(col.r, col.g, col.b, col.a), 0, "weebware");
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



