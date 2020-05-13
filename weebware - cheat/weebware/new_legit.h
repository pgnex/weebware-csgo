#pragma once
#include "Header.h"

class c_newlegit {
public:
	// initittttttt this is our base class for features
	void run(c_usercmd* cmd, c_base_entity* local);

	// features not requiring own class
	void reduce_recoil(c_usercmd* cmd);

	// feature funcs also not requiring own class

	// getter setter for local
	void set_local(c_base_entity* local) { this->m_local = local; }
	c_base_entity* get_local() { return this->m_local; }

private:
	c_base_entity* m_local;

}; extern c_newlegit g_newlegit;


class c_triggerbot : public c_newlegit {
private:
	c_base_entity* m_local;
	long m_last_delay;
	c_base_entity* cur_target = NULL;
	int last_delay_trig = 0;
	QAngle view_angles = QAngle(0.f, 0.f, 0.f);
public:
	void run(c_usercmd* cmd, c_newlegit* legit);
	c_base_entity* get_trace_ent();
	void shoot(c_usercmd* cmd, c_base_entity* target);
	bool raytrace_hc(Vector viewAngles, float chance, c_base_entity* target, float dst);
	std::vector<int> setup_hitboxes();

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