#pragma once
#include "Header.h"

// note: shifting bits into 1 always results in powers of 2, which is which is required for proper bitmasking

// get_target_vector flags (hitscan stuff)
#define FL_IGNORE_HEAD			(1 << 0)
#define FL_IGNORE_LIMBS			(1 << 1)
#define FL_PRIORITIZE_BODY		(1 << 2)
#define FL_DISABLE_MULTIPOINT	(1 << 3)

// get_multipoints flags (multipoint stuff)
#define FL_MULTIPOINT_DISABLED	(1 << 0)
#define FL_MULTIPOINT_HEAD_ONLY (1 << 1)
#define FL_MULTIPOINT_BODY_ONLY (1 << 2)
#define FL_MULTIPOINT_HEAD_BODY (1 << 3)

struct backtrack_data_t;
struct target_vector_t {
	bool initialized = false;
	c_base_entity* owner;
	int hitbox;
	int bone;
	float damage;
	Vector position;
};

class c_ragebot {
public:

	bool Run(c_usercmd* cmd);
	target_vector_t get_target_vector(c_base_entity* entity, int flags, int multipoint_flags = 0);
	target_vector_t rage_backtrack(c_base_entity* player, backtrack_data_t& used_record);
	int get_target_flags(c_base_entity* player);
	void quick_stop(c_usercmd* cmd);
	std::vector<Vector> hitpoints[64];
	long get_epoch();

	long long best_target_found = -1;
	int best_target_index = -1;
	Vector best_target_vector;


private:

	std::vector<target_vector_t> c_ragebot::get_multipoints(c_base_entity* entity, mstudiobbox_t* bbox, target_vector_t original_target, int flags = 0);
	bool velocity_check(c_basecombat_weapon* weapon);
	bool hit_chance(float flChance, c_basecombat_weapon* weapon);
	bool aim_step(QAngle& angle);
	bool player_valid(c_base_entity* player);
	float forward;
	float sidemove;

};

extern c_ragebot* ragebot;