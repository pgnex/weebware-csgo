#pragma once
#include "Header.h"

class c_d3dxesp
{
public:

	enum fonts {
		tahoma = 0,
		tahoma_large
	};

	void d9esp_main(IDirect3DDevice9* pDevice);
	void d3dx_reset();
	void on_lost_device();
	void get_device(IDirect3DDevice9* pDevice);

private:
	c_base_entity* local;

	void water_mark();
	void draw_inaccuracy_circle();
	void draw_sniper_crosshair();
	void recoil_crosshair();
	void bomb_timer(c_base_entity* ent);
	void spectator_list(c_base_entity* ent);
	void render_box(s_boundaries bounds, c_base_entity* ent, bool is_visible);
	void render_box_corners(s_boundaries bounds, c_base_entity* ent, bool is_visible);
	void render_skeleton(c_base_entity* ent, bool is_visible);
	void render_health(s_boundaries bounds, c_base_entity* ent);
	void render_name(s_boundaries bounds, c_base_entity* ent, bool is_visible);
	void render_weapon(s_boundaries bounds, c_base_entity* ent);
	void render_ammo(s_boundaries bounds, c_base_entity* ent);
	void defusing_indicator(s_boundaries bounds, c_base_entity* ent);
	void display_backtrack();

	int spec_count = 0;
	w2s_players w2s_player[99];
};

extern c_d3dxesp g_d3dxesp;