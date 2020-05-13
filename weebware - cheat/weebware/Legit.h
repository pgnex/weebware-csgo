 #pragma once
#include "Header.h"

class c_legitbot
{
public:
	c_base_entity * m_local;
	c_basecombat_weapon * g_weapon;
	void create_move(c_usercmd* cmd);

	// Expose this function so accuracy boost can use it.
	Vector center_hitbox(c_base_entity* ent, int id);


	void triggerbot_main(c_usercmd* cmd);
	bool sniper_scoped();
	void magnet_triggerbot(c_usercmd* cmd);
	QAngle magnet_hitbox(c_base_entity* target);
	void auto_stop(c_usercmd* cmd);
	c_base_entity* closest_target_triggerbot();
	bool is_visible_angle(c_base_entity* target, Vector dst2);
	std::vector<int> get_triggerbot_hitboxes();


private:
	bool raytrace_hc(Vector viewAngles, float chance, c_base_entity* target, float dst);
	c_base_entity * closest_target_available();
	QAngle closest_hitbox(c_base_entity* target);
	QAngle rcs_scaled(QAngle original_angle, float pitch, float yaw);
	void standalone_rcs(c_usercmd* cmd);
	long get_epoch();
	double m_last_time = 0;
	bool next_attack_queued();

};

extern c_legitbot g_legitbot;
