#pragma once

enum ai_flags
{
	standby = 0,
	travel = 1 << 0,
	aggravated = 1 << 1,
	kill = 1 << 2
};


enum ai_movement
{
	nothing,
	random,
	follow_team,
	plant_bomb,
	defend,
	defuse_bomb,
	hide
};


class c_ai
{
public:
	void create_move(c_usercmd* cmd, c_base_entity* local);
	void auto_queue();
	bool needs_queue = true;

private:
	c_base_entity* m_local;

	c_base_entity* m_target_ent;

	// Flags for what to do with AI
	ai_flags m_flags;

	void adjust_to_velocity(c_usercmd* cmd);

	void jump_on_low_velocity(c_usercmd* cmd);

	float m_tempt_fov = 180.f;

	void correct_range(c_usercmd* cmd);

	QAngle m_localview;

	void auto_buy_weapons(c_usercmd* cmd);

	void kill(c_usercmd* cmd);
};
 
extern c_ai g_ai;