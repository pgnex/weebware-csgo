#pragma once
#include "Header.h"


class c_config_list
{
public:
	std::vector<std::string> config_names;
	void update_all_configs();
	void save_weebware_config();
	void save_existing_weebware();
	void load_weebware_config();
	void delete_weebware_config();
	char cur_save_name[256];
	std::string cur_load_name = "";

};

extern c_config_list g_config_list;


class c_legit_cfg
{
public:
	int enable_legitbot;
	int legitbot_activation_key;
	float maximum_fov;
	float sensitivity;
	float reaction_time;
	float pitch_rcs;
	float yaw_rcs;
	bool quick_stop;
	bool aim_through_smoke;
	bool aim_while_blind;
	bool hitbox_head = true;
	bool hitbox_chest;
	bool hitbox_stomach;
	bool standalone_rcs;
	float standalone_rcs_power = 60.f;
	int accuracy_boost;

	int triggerbot_active;
	int triggerbot_key;
	bool triggerbot_head;
	bool triggerbot_chest;
	bool triggerbot_stomach;
	float triggerbot_hitchance;
};


class c_rage_cfg
{
public:
	bool enable_ragebot;

};
	
class c_vis_cfg
{
public:
	int enable_visuals;
	int enable_visuals_key;
	bool visuals_watermark;
	bool visuals_teammates;
	bool visuals_bounding_box;
	bool visuals_health_bars;
	bool visuals_inacc_circle;
	bool visuals_backtrack_dots;
	bool visuals_visible_only;
	bool visuals_bspotted;
	bool visuals_bomb_timer;
	bool visuals_dormant_esp;
};

class c_misc_cfg
{
public:
	bool enable_misc;
	bool auto_jump;
	bool misc_ai;
	float misc_ai_legitfactor;
	bool misc_ai_random;
	bool misc_ai_nearest;
	bool misc_ai_defuse;
	bool misc_ai_defend;
};

class c_sets_cfg
{
public:
	ImVec4 water_mark = ImVec4(113, 221, 229, 255);
	ImVec4 visuals_bounding = ImVec4(255, 0, 0, 255);
	ImVec4 visuals_bounding_team = ImVec4(255, 255, 0, 255);
	ImVec4 visuals_innacc_circle = ImVec4(0, 0, 255, 50);
	ImVec4 visuals_backtrack_col = ImVec4(0, 0, 255, 50);
	ImVec4 visuals_dormant_col = ImVec4(0, 0, 0, 100);
	ImVec4 visuals_dormant_col_team = ImVec4(0, 0, 0, 100);

};

/*
c_weebwarecfg will load the actual config itself
*/

class c_weebwarecfg
{
public:
	int legit_cfg_index;
	c_legit_cfg legit_cfg[8];
	c_rage_cfg rage_cfg;
	c_vis_cfg vis_cfg;
	c_misc_cfg misc_cfg;
	c_sets_cfg sets_cfg;
};

extern c_weebwarecfg g_weebwarecfg;
