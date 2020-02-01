#pragma once
#include "Header.h"

#ifndef VISUALSSHARED
#define VISUALSSHARED

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


Vector center_hitbox(c_base_entity* ent, int id);
s_boundaries calc_boundaries(c_base_entity* Entity);
bool is_visible(c_base_entity* local, c_base_entity* target);

#endif

