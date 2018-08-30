#pragma once
#ifndef CONFIGS
#define CONFIGS

#include "shared.h"

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
	char skinchanger_skinsearch[256];
	char skinchanger_gunsearch[256];

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
	bool silent_aim;

	int triggerbot_active;
	int triggerbot_key;
	bool triggerbot_head;
	bool triggerbot_chest;
	bool triggerbot_stomach;
	float triggerbot_hitchance;
};

class override_skin_style
{
public:
	int weapon_id = 0;
	int m_paint_kit = 0;
	float m_wear = FLT_MIN;
	int m_seed = 69;
};

class c_weebwareskins_save
{
public:
	override_skin_style first[10];
	override_skin_style second[10];
	override_skin_style third[10];
	override_skin_style fourth[5];
};

class c_weebwareskinscfg
{
public:
	override_skin_style skin_wheel[35];
};

class c_weebwarecfg
{
public:

	int legit_cfg_index;
	c_legit_cfg legit_cfg[8];

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
	int visuals_chams;

	bool enable_misc;
	bool auto_jump;
	bool misc_ai;
	float misc_ai_legitfactor;
	bool misc_ai_random;
	bool misc_ai_nearest;
	bool misc_ai_defuse;
	bool misc_ai_defend;

	ImVec4 water_mark_col = ImVec4(113, 221, 229, 255);
	ImVec4 visuals_bounding_col = ImVec4(255, 0, 0, 255);
	ImVec4 visuals_bounding_team_col = ImVec4(255, 255, 0, 255);
	ImVec4 visuals_innacc_circle_col = ImVec4(0, 0, 255, 50);
	ImVec4 visuals_backtrack_col = ImVec4(0, 0, 255, 50);
	ImVec4 visuals_dormant_col = ImVec4(0, 0, 0, 100);
	ImVec4 visuals_dormant_col_team = ImVec4(0, 0, 0, 100);

	ImVec4 visuals_chams_col = ImVec4(255, 0, 0, 255);
	ImVec4 visuals_chams_team_col = ImVec4(0, 255, 255, 255);

	bool visuals_chams_render_team = false;

	bool skinchanger_enabled = false;

	int skinchanger_selected_gun = 0;
	int previous_knife_index = 0;
	int next_knife_index = 0;

	bool visuals_name_esp = 0;
	ImVec4 visuals_name_esp_col = ImVec4(255, 255, 255, 255);
	ImVec4 visuals_name_esp_col_team = ImVec4(255, 255, 255, 255);
	bool skinchanger_apply_nxt = 0;

	bool misc_legit_aa_enabled = 0;
	bool misc_legit_aa_jitter = 0;
	// 0, menu index , 1 config index
	int selected_knife_index[2] = { 0 };
	int selected_gun_index = 0;
	bool use_dynamicfov[8] = { 0 };
};

extern c_weebwareskins_save g_weebwarecfg_skins_but_donottouch;
extern c_weebwarecfg g_weebwarecfg;
extern c_weebwareskinscfg g_weebwareskinscfg;
#endif