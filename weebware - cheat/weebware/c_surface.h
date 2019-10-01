#pragma once

enum materialvarflags_t
{
	material_var_debug = (1 << 0),
	material_var_no_debug_override = (1 << 1),
	material_var_no_draw = (1 << 2),
	material_var_use_in_fillrate_mode = (1 << 3),
	material_var_vertexcolor = (1 << 4),
	material_var_vertexalpha = (1 << 5),
	material_var_selfillum = (1 << 6),
	material_var_additive = (1 << 7),
	material_var_alphatest = (1 << 8),
	material_var_multipass = (1 << 9),
	material_var_znearer = (1 << 10),
	material_var_model = (1 << 11),
	material_var_flat = (1 << 12),
	material_var_nocull = (1 << 13),
	material_var_nofog = (1 << 14),
	material_var_ignorez = (1 << 15),
	material_var_decal = (1 << 16),
	material_var_envmapsphere = (1 << 17),
	material_var_noalphamod = (1 << 18),
	material_var_envmapcameraspace = (1 << 19),
	material_var_basealphaenvmapmask = (1 << 20),
	material_var_translucent = (1 << 21),
	material_var_normalmapalphaenvmapmask = (1 << 22),
	material_var_needs_software_skinning = (1 << 23),
	material_var_opaquetexture = (1 << 24),
	material_var_envmapmode = (1 << 25),
	material_var_suppress_decals = (1 << 26),
	material_var_halflambert = (1 << 27),
	material_var_wireframe = (1 << 28),
	material_var_allowalphatocoverage = (1 << 29),
};

enum efontflags
{
	fontflag_none,
	fontflag_italic = 0x001,
	fontflag_underline = 0x002,
	fontflag_strikeout = 0x004,
	fontflag_symbol = 0x008,
	fontflag_antialias = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary = 0x040,
	fontflag_dropshadow = 0x080,
	fontflag_additive = 0x100,
	fontflag_outline = 0x200,
	fontflag_custom = 0x400,
	fontflag_bitmap = 0x800,
};

class c_color
{
public:
	int r, g, b, a;

	c_color(int R = 0, int G = 0, int B = 0, int A = 0)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}

	c_color(ImVec4 vec_color)
	{
		r = vec_color.x;
		g = vec_color.y;
		b = vec_color.z;
		a = vec_color.w;
	}


};

struct vertex_t
{
public:

	float	x;
	float	y;

	void Init(c_vector_2d pos)
	{
		x = pos.x;
		y = pos.y;
	}
	vertex_t()
	{
	}
	vertex_t(c_vector_2d pos)
	{
		x = pos.x;
		y = pos.y;
	}

private:

	float	tex_x = 0.f;
	float	tex_y = 0.f;
};

class c_surface // : iappsystem
{
public:

	// vitual functions
	virtual void padfunction0() = 0;
	virtual void padfunction1() = 0;
	virtual void padfunction2() = 0;
	virtual void padfunction3() = 0;
	virtual void padfunction4() = 0;
	virtual void padfunction5() = 0;
	virtual void padfunction6() = 0;
	virtual void padfunction7() = 0;
	virtual void padfunction8() = 0;
	virtual void padfunction9() = 0;
	virtual void padfunction10() = 0;
	virtual void padfunction11() = 0;
	virtual void padfunction12() = 0;
	virtual void padfunction13() = 0;
	virtual void padfunction14() = 0;
	virtual void drawsetcolor(int r, int g, int b, int a) = 0;
	virtual void drawfilledrect(int x0, int y0, int x1, int y1) = 0;
	virtual void padfunction17() = 0;
	virtual void drawoutlinedrect(int x0, int y0, int x1, int y1) = 0;
	virtual void drawline(int x0, int y0, int x1, int y1) = 0;
	virtual void drawpolyline(int *x, int *y, int count) = 0;
	virtual void padfunction21() = 0;
	virtual void padfunction22() = 0;
	virtual void drawsettextfont(unsigned long font) = 0;
	virtual void padfunction24() = 0;
	virtual void drawsettextcolor(int r, int g, int b, int a) = 0;
	virtual void drawsettextpos(int x, int y) = 0;
	virtual void padfunction27() = 0;
	virtual void drawprinttext(const wchar_t *text, int textlen, int a = 0) = 0;
	virtual void padfunction29() = 0;
	virtual void padfunction30() = 0;
	virtual void padfunction31() = 0;
	virtual void padfunction32() = 0;
	virtual void padfunction33() = 0;
	virtual void padfunction34() = 0;
	virtual void padfunction35() = 0;
	virtual void padfunction36() = 0;
	virtual void drawsettexturergba(int textureid, unsigned char  const* colors, int w, int h) = 0;
	virtual void drawsettexture(int textureid) = 0;
	virtual void padfunction39() = 0;
	virtual void padfunction40() = 0;
	virtual void drawtexturedrect(int x0, int y0, int x1, int y1) = 0;
	virtual bool istextureidvalid(int id) = 0;
	virtual int createnewtextureid(bool procedural) = 0;
	virtual void padfunction44() = 0;
	virtual void padfunction45() = 0;
	virtual void padfunction46() = 0;
	virtual void padfunction47() = 0;
	virtual void padfunction48() = 0;
	virtual void padfunction49() = 0;
	virtual void padfunction50() = 0;
	virtual void padfunction51() = 0;
	virtual void padfunction52() = 0;
	virtual void padfunction53() = 0;
	virtual void padfunction54() = 0;
	virtual void padfunction55() = 0;
	virtual void padfunction56() = 0;
	virtual void padfunction57() = 0;
	virtual void padfunction58() = 0;
	virtual void padfunction59() = 0;
	virtual void padfunction60() = 0;
	virtual void padfunction61() = 0;
	virtual void padfunction62() = 0;
	virtual void padfunction63() = 0;
	virtual void padfunction64() = 0;
	virtual void padfunction65() = 0;
	virtual void unlockcursor() = 0;// 66
	virtual void lockcursor() = 0; // 67
	virtual void padfunction68() = 0;
	virtual void padfunction69() = 0;
	virtual void padfunction70() = 0;
	virtual unsigned long create_font() = 0;
	virtual bool setfontglyphset(unsigned long font, const char *windowsfontname, int tall, int weight, int blur, int scanlines, int flags, int nrangemin = 0, int nrangemax = 0) = 0;
	virtual void padfunction73() = 0;
	virtual void padfunction74() = 0;
	virtual void padfunction75() = 0;
	virtual void padfunction76() = 0;
	virtual void padfunction77() = 0;
	virtual void padfunction78() = 0;
	virtual void gettextsize(unsigned long font, const wchar_t *text, int &wide, int &tall) = 0;
	virtual void padfunction80() = 0;
	virtual void padfunction81() = 0;
	virtual void padfunction82() = 0;
	virtual void padfunction83() = 0;
	virtual void padfunction84() = 0;
	virtual void padfunction85() = 0;
	virtual void padfunction86() = 0;
	virtual void padfunction87() = 0;
	virtual void padfunction88() = 0;
	virtual void padfunction89() = 0;
	virtual void padfunction90() = 0;
	virtual void padfunction91() = 0;
	virtual void padfunction92() = 0;
	virtual void padfunction93() = 0;
	virtual void padfunction94() = 0;
	virtual void padfunction95() = 0;
	virtual void padfunction96() = 0;
	virtual void padfunction97() = 0;
	virtual void padfunction98() = 0;
	virtual void padfunction99() = 0;
	virtual void surfacegetcursorpos(int& x, int& y) = 0;
	virtual void surfacesetcursorpos(int x, int y) = 0;
	virtual void padfunction102() = 0;
	virtual void drawoutlinedcircle(int x, int y, int r, int seg) = 0;
	virtual void padfunction104() = 0;
	virtual void padfunction105() = 0;
	virtual void drawtexturedpolygon(int vtxcount, vertex_t *vtx, bool bclipvertices = false) = 0;
	virtual void padfunction107() = 0;
	virtual void padfunction108() = 0;
	virtual void padfunction109() = 0;
	virtual void padfunction110() = 0;
	virtual void padfunction111() = 0;
	virtual void padfunction112() = 0;
	virtual void padfunction113() = 0;
	virtual void padfunction114() = 0;
	virtual void padfunction115() = 0;
	virtual void padfunction116() = 0;
	virtual void padfunction117() = 0;
	virtual void padfunction118() = 0;
	virtual void padfunction119() = 0;
	virtual void padfunction120() = 0;
	virtual void padfunction121() = 0;
	virtual void padfunction122() = 0;
	virtual void padfunction123() = 0;
	virtual void padfunction124() = 0;
	virtual void padfunction125() = 0;
	virtual void padfunction126() = 0;
	virtual void padfunction127() = 0;
	virtual void padfunction128() = 0;
	virtual void padfunction129() = 0;
	virtual void padfunction130() = 0;
	virtual void padfunction131() = 0;
	virtual void padfunction132() = 0;
	virtual void padfunction133() = 0;
	virtual void padfunction134() = 0;
	virtual void padfunction135() = 0;
	virtual void padfunction136() = 0;
	virtual void padfunction137() = 0;
	virtual void padfunction138() = 0;
	virtual void padfunction139() = 0;
	virtual void padfunction140() = 0;
	virtual void padfunction141() = 0;
	virtual void padfunction142() = 0;
	virtual void padfunction143() = 0;
	virtual void padfunction144() = 0;
	virtual void padfunction145() = 0;
	virtual void padfunction146() = 0;
	virtual void padfunction147() = 0;
	virtual void padfunction148() = 0;
	virtual void padfunction149() = 0;
	virtual void padfunction150() = 0;
	virtual void padfunction151() = 0;
	virtual void padfunction152() = 0;
	virtual void padfunction153() = 0;
	virtual void padfunction154() = 0;
	virtual void padfunction155() = 0;
	virtual void padfunction156() = 0;
	virtual void padfunction157() = 0;
	virtual void padfunction158() = 0;
	virtual void padfunction159() = 0;
	virtual void padfunction160() = 0;
	virtual void padfunction161() = 0;
	virtual void drawcoloredcircle(int centerx, int centery, float radius, int r, int g, int b, int a) = 0;
	virtual void padfunction163() = 0;
	virtual void padfunction164() = 0;
	virtual void padfunction165() = 0;
	virtual void padfunction166() = 0;
	virtual void padfunction167() = 0;
	virtual void padfunction168() = 0;
	virtual void padfunction169() = 0;
	virtual void padfunction170() = 0;
	virtual void padfunction171() = 0;
	virtual void padfunction172() = 0;
	virtual void padfunction173() = 0;
	virtual void padfunction174() = 0;
	virtual void padfunction175() = 0;
	virtual void padfunction176() = 0;
	virtual void padfunction177() = 0;
	virtual void padfunction178() = 0;
	virtual void padfunction179() = 0;
	virtual void padfunction180() = 0;
	virtual void padfunction181() = 0;
	virtual void padfunction182() = 0;
	virtual void padfunction183() = 0;
	virtual void padfunction184() = 0;
	virtual void padfunction185() = 0;
	virtual void padfunction186() = 0;
	virtual void padfunction187() = 0;
	virtual void padfunction188() = 0;
	virtual void padfunction189() = 0;
	virtual void padfunction190() = 0;
	virtual void padfunction191() = 0;
	virtual void padfunction192() = 0;
	virtual void padfunction193() = 0;
	virtual void padfunction194() = 0;
	virtual void padfunction195() = 0;
	virtual void padfunction196() = 0;
	virtual void padfunction197() = 0;
	virtual void padfunction198() = 0;
	virtual void padfunction199() = 0;
	virtual void padfunction200() = 0;
	virtual void padfunction201() = 0;
	virtual void padfunction202() = 0;
	virtual void padfunction203() = 0;
	virtual void padfunction204() = 0;
	virtual void padfunction205() = 0;
	virtual void padfunction206() = 0;
	virtual void padfunction207() = 0;
	virtual void padfunction208() = 0;
	virtual void padfunction209() = 0;
	virtual void padfunction210() = 0;
	virtual void padfunction211() = 0;
	virtual void padfunction212() = 0; // vmt count = 212

	void drawtexturedpoly(int n, vertex_t* vertice, c_color col) {
		static int texture_id = createnewtextureid(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		drawsettexturergba(texture_id, buf, 1, 1);
		drawsetcolor(col.r, col.g, col.b, col.a);
		drawsettexture(texture_id);
		drawtexturedpolygon(n, vertice);
	}


	void drawfilledcircle(float x, float y, c_color color, float radius, float points) {
		std::vector<vertex_t> vertices;
		float step = (float)PI * 2.0f / points;

		for (float a = 0; a < (PI * 2.0f); a += step)
			vertices.push_back(vertex_t({ radius * cosf(a) + x, radius * sinf(a) + y }));

		drawtexturedpoly((int)points, vertices.data(), color);
	}
};

class c_panel
{
public:
	unsigned int getpanel(int type)
	{
		return getvfunc<unsigned int(__thiscall*)(void*, int)>(this, 1)(this, type);
	}

	const char* getname(unsigned int vguipanel)
	{
		return getvfunc<const char* (__thiscall*)(PVOID, unsigned int)>(this, 36)(this, vguipanel);
	}

	void set_mouseinput_enabled(unsigned int panel, bool state)
	{
		return getvfunc<void(__thiscall *)(PVOID, int, bool)>(this, 32)(this, panel, state);
	}

};