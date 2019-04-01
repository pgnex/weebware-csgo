#pragma once
#include "Header.h"

struct s_boundaries
{
	int x = 0, y = 0, h = 0, w = 0;
	bool has_w2s = false;
	bool dormant = false;
};

struct w2s_players
{
	s_boundaries boundary;
	Vector w2s_backtracked;
};

struct backtrack_skeleton
{
	Vector parent[125] = { Vector(0,0,0) };
	Vector child[125] = { Vector(0,0,0) };
	Vector w2s_parent[125] = { Vector(0,0,0) };
	Vector w2s_child[125] = { Vector(0,0,0) };
	int bone_count;
	bool has_w2s_parent[125] = { false };
	bool has_w2s_child[125] = { false };
};

class c_esp
{
public:
	void esp_main();
	void esp_reset();

	// w2s needs to be done in paint
	w2s_players w2s_player[99];
	backtrack_skeleton m_skeleton_backtrack;
	void calc_w2svalues();
private:
	c_base_entity * local;
	void water_mark();
	Vector center_hitbox(c_base_entity* ent, int id);
	s_boundaries calc_boundaries(c_base_entity* Entity);
	void render_box(s_boundaries bounds, c_base_entity* ent, bool is_team);
	void render_box_corners();
	void render_skeleton(c_base_entity* ent, bool is_team);
	void render_health(s_boundaries bounds, c_base_entity* ent, bool is_team);
	void draw_inaccuracy_circle();
	void render_name(s_boundaries bounds, c_base_entity* ent, bool is_team);
	void bomb_timer(c_base_entity* ent);
	void draw_crosshair();
	void recoil_crosshair();

	bool c_esp::is_visible(c_base_entity* local, c_base_entity* target);
	void display_backtrack();
};

extern c_esp g_esp;