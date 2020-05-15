#pragma once
#include "Header.h"

class c_newlegit {
public:
	// initittttttt this is our base class for features
	void run(c_usercmd* cmd, c_base_entity* local);

	// features not requiring own class
	void reduce_recoil(c_usercmd* cmd);

	// shared feature funcs not requiring own class
	c_base_entity* closest_target_available(bool triggerbot);

	c_base_entity* m_local;
private:


}; extern c_newlegit g_newlegit;

class c_aimbot : public  c_newlegit {
private:
	// feature vars
	long m_last_delay;
	c_base_entity* cur_target = NULL;
	int last_delay_aim = 0;
public:
	// init feature..
	void run(c_usercmd* cmd);

	// feature funcs
	void auto_stop(c_usercmd* cmd);
	QAngle closest_hitbox(c_base_entity* target, bool triggerbot);
	std::vector<int> setup_hitboxes(bool triggerbot);
	bool next_attack_queued();
	void do_aim_stuffs(c_base_entity* target, c_usercmd* cmd, bool triggerbot);
};


class c_triggerbot : public c_newlegit, public c_aimbot {
private:
	// feature vars
	long m_last_delay;
	c_base_entity* cur_target = NULL;
	int last_delay_trig = 0;
	QAngle view_angles = QAngle(0.f, 0.f, 0.f);
public:
	// init feature..
	void run(c_usercmd* cmd);

	// feature funcs
	c_base_entity* get_trace_ent();
	std::vector<int> setup_hitboxes();
	void shoot(c_usercmd* cmd, c_base_entity* target);
	bool raytrace_hc(Vector viewAngles, float chance, c_base_entity* target, float dst);
	bool sniper_scoped();
	bool next_attack_queued();
};

/*
these are the hitboxes for triggerbot
HITGROUP_GENERIC    0
HITGROUP_HEAD       1
HITGROUP_CHEST      2
HITGROUP_STOMACH    3
HITGROUP_LEFTARM    4
HITGROUP_RIGHTARM   5
HITGROUP_LEFTLEG    6
HITGROUP_RIGHTLEG   7
HITGROUP_GEAR       10
*/