#pragma once
#ifndef CONFIGS
#define CONFIGS

#include "shared.h"
#include "json.h"

using nlohmann::json;


static bool check(std::string key, json data) {
	if (data.count(key) > 0) {
		return true;
	}
	return false;
}

static bool check_color(std::string key, json data) {
	if (data.count(key + "a") > 0) {
		return true;
	}
	return false;
}

class c_config_list
{
public:
	// config browser
	std::vector<std::string> config_browser_info;
	json config_browser_buffer;
	void update_config_browser();
	void load_browser_config();
	void save_browser_config();
	std::string cur_secret = "";
	std::string cur_desc = "Description: ";
	std::string cur_creator = "Created by: ";
	std::string cur_config_browser_name = "";

	// configs
	std::vector<std::string> config_names;
	void update_all_configs();
	void save_weebware_config();
	void save_existing_weebware();
	void load_weebware_config(std::string load_name);
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
	bool triggerbot_head = true;
	bool triggerbot_chest;
	bool triggerbot_stomach;
	bool triggerbot_arms;
	bool triggerbot_legs;
	float triggerbot_hitchance;
	float legit_maximum_ticks = 12;
	bool use_dynamicfov;
	float triggerbot_reaction;
	bool target_teammates;
	bool magnet_triggerbot_enabled;
	float magnet_trigger_smooth;
	float magnet_trigger_fov;

	json convert()
	{
		json tmp;
		tmp["maximum_fov"] = maximum_fov;
		tmp["enable_legitbot"] = enable_legitbot;
		tmp["legitbot_activation_key"] = legitbot_activation_key;
		tmp["sensitivity"] = sensitivity;
		tmp["reaction_time"] = reaction_time;
		tmp["pitch_rcs"] = pitch_rcs;
		tmp["yaw_rcs"] = yaw_rcs;
		tmp["quick_stop"] = quick_stop;
		tmp["aim_through_smoke"] = aim_through_smoke;
		tmp["aim_while_blind"] = aim_while_blind;
		tmp["hitbox_head"] = hitbox_head;
		tmp["hitbox_chest"] = hitbox_chest;
		tmp["hitbox_stomach"] = hitbox_stomach;
		tmp["standalone_rcs"] = standalone_rcs;
		tmp["standalone_rcs_power"] = standalone_rcs_power;
		tmp["accuracy_boost"] = accuracy_boost;
		tmp["silent_aim"] = silent_aim;
		tmp["triggerbot_active"] = triggerbot_active;
		tmp["triggerbot_key"] = triggerbot_key;
		tmp["triggerbot_head"] = triggerbot_head;
		tmp["triggerbot_chest"] = triggerbot_chest;
		tmp["triggerbot_stomach"] = triggerbot_stomach;
		tmp["triggerbot_arms"] = triggerbot_arms;
		tmp["triggerbot_legs"] = triggerbot_legs;
		tmp["triggerbot_hitchance"] = triggerbot_hitchance;
		tmp["legit_maximum_ticks"] = legit_maximum_ticks;
		tmp["use_dynamicfov"] = use_dynamicfov;
		tmp["triggerbot_reaction"] = triggerbot_reaction;
		tmp["target_teammates"] = target_teammates;
		tmp["magnet_triggerbot_enabled"] = magnet_triggerbot_enabled;
		tmp["magnet_trigger_smooth"] = magnet_trigger_smooth;
		tmp["magnet_trigger_fov"] = magnet_trigger_fov;
		return tmp;
	}

	void convert(json data)
	{
		if (check("maximum_fov", data)) maximum_fov = data["maximum_fov"];
		if (check("enable_legitbot", data)) enable_legitbot = data["enable_legitbot"];
		if (check("legitbot_activation_key", data)) legitbot_activation_key = data["legitbot_activation_key"];
		if (check("sensitivity", data)) sensitivity = data["sensitivity"];
		if (check("reaction_time", data)) reaction_time = data["reaction_time"];
		if (check("pitch_rcs", data)) pitch_rcs = data["pitch_rcs"];
		if (check("yaw_rcs", data)) yaw_rcs = data["yaw_rcs"];
		if (check("quick_stop", data)) quick_stop = data["quick_stop"];
		if (check("aim_through_smoke", data)) aim_through_smoke = data["aim_through_smoke"];
		if (check("aim_while_blind", data)) aim_while_blind = data["aim_while_blind"];
		if (check("hitbox_head", data)) hitbox_head = data["hitbox_head"];
		if (check("hitbox_chest", data)) hitbox_chest = data["hitbox_chest"];
		if (check("hitbox_stomach", data)) hitbox_stomach = data["hitbox_stomach"];
		if (check("standalone_rcs", data)) standalone_rcs = data["standalone_rcs"];
		if (check("standalone_rcs_power", data)) standalone_rcs_power = data["standalone_rcs_power"];
		if (check("accuracy_boost", data)) accuracy_boost = data["accuracy_boost"];
		if (check("silent_aim", data)) silent_aim = data["silent_aim"];
		if (check("triggerbot_active", data)) triggerbot_active = data["triggerbot_active"];
		if (check("triggerbot_key", data)) triggerbot_key = data["triggerbot_key"];
		if (check("triggerbot_head", data)) triggerbot_head = data["triggerbot_head"];
		if (check("triggerbot_chest", data)) triggerbot_chest = data["triggerbot_chest"];
		if (check("triggerbot_stomach", data)) triggerbot_stomach = data["triggerbot_stomach"];
		if (check("triggerbot_arms", data)) triggerbot_arms = data["triggerbot_arms"];
		if (check("triggerbot_legs", data)) triggerbot_legs = data["triggerbot_legs"];
		if (check("triggerbot_hitchance", data)) triggerbot_hitchance = data["triggerbot_hitchance"];
		if (check("legit_maximum_ticks", data)) legit_maximum_ticks = data["legit_maximum_ticks"];
		if (check("use_dynamicfov", data)) use_dynamicfov = data["use_dynamicfov"];
		if (check("triggerbot_reaction", data)) triggerbot_reaction = data["triggerbot_reaction"];
		if (check("target_teammates", data)) target_teammates = data["target_teammates"];
		if (check("magnet_triggerbot_enabled", data)) magnet_triggerbot_enabled = data["magnet_triggerbot_enabled"];
		if (check("magnet_trigger_smooth", data)) magnet_trigger_smooth = data["magnet_trigger_smooth"];
		if (check("magnet_trigger_fov", data)) magnet_trigger_fov = data["magnet_trigger_fov"];
	}

};


class override_skin_style
{
public:
	int weapon_id = 0;
	int m_paint_kit = 0;
	float m_wear = FLT_MIN;
	int m_seed = 69;

	json convert()
	{
		json tmp;
		tmp["weapon_id"] = weapon_id;
		tmp["m_paint_kit"] = m_paint_kit;
		tmp["m_wear"] = m_wear;
		tmp["m_seed"] = m_seed;
		return tmp;
	}

	void convert(json data)
	{
		if (check("weapon_id", data)) weapon_id = data["weapon_id"];
		if (check("m_paint_kit", data)) m_paint_kit = data["m_paint_kit"];
		if (check("m_wear", data)) m_wear = data["m_wear"];
		if (check("m_seed", data)) m_seed = data["m_seed"];
	}
};


class c_weebwarecfg
{
public:
	void save_color(ImVec4& color, json& data, std::string var_name)
	{
		data[var_name + "r"] = color.x;
		data[var_name + "g"] = color.y;
		data[var_name + "b"] = color.z;
		data[var_name + "a"] = color.w;
	}

	void read_color(ImVec4& color, json& data, std::string var_name)
	{
		color.x = data[var_name + "r"];
		color.y = data[var_name + "g"];
		color.z = data[var_name + "b"];
		color.w = data[var_name + "a"];
	}

	int legit_cfg_index;
	c_legit_cfg legit_cfg[8];
	override_skin_style skin_wheel[35];


	int enable_visuals;
	int enable_visuals_key;
	bool visuals_watermark = true;
	bool visuals_teammates;
	bool visuals_bounding_box;
	bool visuals_corner_box;
	bool visuals_health_bars;
	bool visuals_inacc_circle;
	bool visuals_fov_circle;
	bool visuals_sniper_crosshair;
	bool visuals_recoil_crosshair;
	bool visuals_backtrack_dots;
	bool visuals_visible_only;
	bool visuals_bspotted;
	bool visuals_bomb_timer;
	bool visuals_dormant_esp;
	int visuals_chams;
	bool visuals_glow_enabled;
	bool visuals_glow_player;
	bool visuals_glow_team;
	bool visuals_glow_hidden_col;
	bool visuals_glow_c4;
	bool visuals_glow_chicken;
	bool visuals_glow_weapon;
	bool visuals_skeleton;
	bool enable_misc;
	bool enable_bullet_tracers;
	float bullet_tracer_expire = 1.f;
	bool auto_jump;
	int auto_jump_hitchance = 100;
	bool night_sky;
	bool remove_flash;
	bool screenshot_proof;
	bool disable_post_processing;

	int auto_strafe = 0;
	bool edge_jump;
	int edge_jump_key = 0;
	bool duck_in_air;
	bool misc_ai;
	float misc_ai_legitfactor;
	bool misc_ai_random;
	bool misc_ai_nearest;
	bool misc_ai_defuse;
	bool misc_ai_defend;
	bool visuals_chams_render_team = false;
	bool skinchanger_enabled = false;
	int skinchanger_selected_gun = 0;
	int previous_knife_index = 0;
	int next_knife_index = 0;
	bool visuals_name_esp = 0;
	bool visuals_weapon_esp = 0;
	bool skinchanger_apply_nxt = 0;
	bool misc_legit_aa_enabled = 0;
	bool misc_legit_aa_jitter = 0;
	bool misc_legit_aa_edge = false;
	// 0, menu index , 1 config index
	int selected_knife_index[2] = { 0 };
	int selected_gun_index = 0;
	int visuals_backtrack_style = 0;
	int anti_triggerbot = 0;
	bool misc_clantag_changer = 0;
	bool misc_chat_spammer = 0;
	bool misc_slidewalk = 0;
	int anti_triggerbot_key = 0;
	bool misc_legit_aa_resolver = 0;
	float misc_ai_rotationspeed = 10.f;
	float misc_ai_aimspeed = 30.f;
	bool visuals_nightmode = 0;
	bool wireframe_smoke = 0;
	bool visuals_chams_xqz = 0;
	bool visuals_hitmarkers = 0;
	int hitmarker_sound = 0;
	bool rank_reveal = false;
	bool misc_autoAccept = false;
	bool minecraft_pickaxe = false;
	int anime_model = false;
	bool thirdperson;
	bool killsay;
	bool viewmodel_changer;
	int viewmodel_offset = 0;
	int glove_model;
	int glove_skin;

	ImVec4 water_mark_col = ImVec4(113, 221, 229, 255);
	ImVec4 visuals_bounding_col = ImVec4(255, 0, 0, 255);
	ImVec4 visuals_bounding_team_col = ImVec4(255, 255, 0, 255);
	ImVec4 visuals_innacc_circle_col = ImVec4(0, 0, 255, 50);
	ImVec4 visuals_fov_circle_col = ImVec4(255, 0, 255, 255);
	ImVec4 visuals_backtrack_col = ImVec4(0, 0, 255, 50);
	ImVec4 visuals_dormant_col = ImVec4(0, 0, 0, 100);
	ImVec4 visuals_dormant_col_team = ImVec4(0, 0, 0, 100);
	ImVec4 visuals_bullet_tracer_col = ImVec4(138, 43, 226, 255);
	ImVec4 visuals_sniper_crosshair_col = ImVec4(255, 0, 0, 255);
	ImVec4 visuals_recoil_crosshair_col = ImVec4(255, 0, 255, 255);

	ImVec4 visuals_chams_col = ImVec4(255, 0, 0, 255);
	ImVec4 visuals_chams_team_col = ImVec4(0, 255, 255, 255);

	ImVec4 visuals_name_esp_col = ImVec4(255, 255, 255, 255);
	ImVec4 visuals_name_esp_col_team = ImVec4(255, 255, 255, 255);

	ImVec4 team_visible_col = ImVec4(0, 100, 255, 255);
	ImVec4 team_hidden_col = ImVec4(255, 0, 255, 255);

	// new box esp colors
	ImVec4 visuals_bounding_col_visible = ImVec4(50, 255, 0, 255);
	ImVec4 visuals_bounding_col_hidden = ImVec4(255, 50, 50, 255);

	// corner box esp colors
	ImVec4 visuals_corner_col_visible = ImVec4(104, 66, 244, 255);
	ImVec4 visuals_corner_col_hidden = ImVec4(214, 57, 89, 255);

	// skeleton esp colors
	ImVec4 visuals_skeleton_col_visible = ImVec4(50, 255, 0, 255);
	ImVec4 visuals_skeleton_col_hidden = ImVec4(255, 50, 50, 255);


	// new name esp colors
	ImVec4 visuals_name_esp_col_visible = ImVec4(255, 255, 255, 255);
	ImVec4 visuals_name_esp_col_hidden = ImVec4(255, 255, 0, 255);
	ImVec4 visuals_chams_col_xqz = ImVec4(0, 255, 0, 255);
	ImVec4 visuals_chams_team_col_xqz = ImVec4(0, 0, 255, 255);
	ImVec4 visuals_hitmarker_col = ImVec4(255, 255, 255, 255);
	ImVec4 nightmode_col = ImVec4(12, 12, 12, 255);

	// glow esp color
	ImVec4 visuals_glow_player_col_visible = ImVec4(45, 65, 123, 255);
	ImVec4 visuals_glow_player_col_hidden = ImVec4(255, 65, 123, 255);
	ImVec4 visuals_glow_c4_col = ImVec4(204, 54, 46, 255);
	ImVec4 visuals_glow_chicken_col = ImVec4(73, 244, 82, 255);
	ImVec4 visuals_glow_weapon_col = ImVec4(212, 244, 66, 255);
	ImVec4 visuals_glow_team_col = ImVec4(0, 244, 66, 255);

	json convert()
	{
		// const char* weapon_groups[] = { "Pistols", "Rifles", "SMG","Shotguns", "Heavy", "Auto-Snipers", "AWP", "SSG08" };

		json tmp;
		tmp["pistols"] = legit_cfg[0].convert();
		tmp["rifle"] = legit_cfg[1].convert();
		tmp["smg"] = legit_cfg[2].convert();
		tmp["shotgun"] = legit_cfg[3].convert();
		tmp["heavy"] = legit_cfg[4].convert();
		tmp["auto"] = legit_cfg[5].convert();
		tmp["awp"] = legit_cfg[6].convert();
		tmp["scout"] = legit_cfg[7].convert();
		tmp["legit_cfg_index"] = legit_cfg_index;

		// visuals
		tmp["enable_visuals"] = enable_visuals;
		tmp["enable_visuals_key"] = enable_visuals_key;
		tmp["visuals_watermark"] = visuals_watermark;
		tmp["visuals_teammates"] = visuals_teammates;
		tmp["visuals_bounding_box"] = visuals_bounding_box;
		tmp["visuals_health_bars"] = visuals_health_bars;
		tmp["visuals_inacc_circle"] = visuals_inacc_circle;
		tmp["visuals_backtrack_dots"] = visuals_backtrack_dots;
		tmp["visuals_visible_only"] = visuals_visible_only;
		tmp["visuals_bspotted"] = visuals_bspotted;
		tmp["visuals_bomb_timer"] = visuals_bomb_timer;
		tmp["visuals_dormant_esp"] = visuals_dormant_esp;
		tmp["visuals_chams"] = visuals_chams;
		tmp["visuals_glow_enabled"] = visuals_glow_enabled;
		tmp["visuals_glow_player"] = visuals_glow_player;
		tmp["visuals_glow_c4"] = visuals_glow_c4;
		tmp["visuals_glow_chicken"] = visuals_glow_chicken;
		tmp["visuals_glow_hidden_col"] = visuals_glow_hidden_col;
		tmp["visuals_corner_box"] = visuals_corner_box;
		tmp["visuals_glow_weapon"] = visuals_glow_weapon;
		tmp["visuals_fov_circle"] = visuals_fov_circle;
		tmp["screenshot_proof"] = screenshot_proof;
		tmp["disable_post_processing"] = disable_post_processing;
		tmp["visuals_weapon_esp"] = visuals_weapon_esp;
		tmp["enable_bullet_tracers"] = enable_bullet_tracers;
		tmp["bullet_tracer_expire"] = bullet_tracer_expire;
		tmp["visuals_glow_team"] = visuals_glow_team;

		// misc
		tmp["enable_misc"] = enable_misc;
		tmp["auto_jump"] = auto_jump;
		tmp["autu_jump_hitchance"] = auto_jump_hitchance;
		tmp["misc_ai"] = misc_ai;
		tmp["misc_ai_legitfactor"] = misc_ai_legitfactor;
		tmp["misc_ai_random"] = misc_ai_random;
		tmp["misc_ai_nearest"] = misc_ai_nearest;
		tmp["misc_ai_defuse"] = misc_ai_defuse;
		tmp["misc_ai_defend"] = misc_ai_defend;
		tmp["visuals_chams_render_team"] = visuals_chams_render_team;
		tmp["skinchanger_enabled"] = skinchanger_enabled;
		tmp["skinchanger_selected_gun"] = skinchanger_selected_gun;
		tmp["previous_knife_index"] = previous_knife_index;
		tmp["next_knife_index"] = next_knife_index;
		tmp["visuals_name_esp"] = visuals_name_esp;
		tmp["skinchanger_apply_nxt"] = skinchanger_apply_nxt;
		tmp["misc_legit_aa_enabled"] = misc_legit_aa_enabled;
		tmp["misc_legit_aa_jitter"] = misc_legit_aa_jitter;
		tmp["misc_legit_aa_edge"] = misc_legit_aa_edge;
		tmp["selected_knife_index0"] = selected_knife_index[0];
		tmp["selected_knife_index1"] = selected_knife_index[1];
		tmp["selected_gun_index"] = selected_gun_index;
		tmp["visuals_backtrack_style"] = visuals_backtrack_style;
		tmp["anti_triggerbot"] = anti_triggerbot;
		tmp["misc_clantag_changer"] = misc_clantag_changer;
		tmp["misc_chat_spammer"] = misc_chat_spammer;
		tmp["misc_slidewalk"] = misc_slidewalk;
		tmp["anti_triggerbot_key"] = anti_triggerbot_key;
		tmp["misc_legit_aa_resolver"] = misc_legit_aa_resolver;
		tmp["misc_ai_rotationspeed"] = misc_ai_rotationspeed;
		tmp["misc_ai_aimspeed"] = misc_ai_aimspeed;
		tmp["visuals_nightmode"] = visuals_nightmode;
		tmp["visuals_chams_xqz"] = visuals_chams_xqz;
		tmp["visuals_hitmarkers"] = visuals_hitmarkers;
		tmp["hitmarker_sound"] = hitmarker_sound;
		tmp["rank_reveal"] = rank_reveal;
		tmp["misc_autoAccept"] = misc_autoAccept;
		tmp["minecraft_pickaxe"] = minecraft_pickaxe;
		tmp["reina_model"] = anime_model;
		tmp["auto_strafe"] = auto_strafe;
		tmp["thirdperson"] = thirdperson;
		tmp["killsay"] = killsay;
		tmp["wireframe_smoke"] = wireframe_smoke;
		tmp["visuals_skeleton"] = visuals_skeleton;
		tmp["visuals_recoil_crosshair"] = visuals_recoil_crosshair;
		tmp["visuals_sniper_crosshair"] = visuals_sniper_crosshair;
		tmp["viewmodel_changer"] = viewmodel_changer;
		tmp["viewmodel_offset"] = viewmodel_offset;
		tmp["edge_jump"] = edge_jump;
		tmp["edge_jump_key"] = edge_jump_key;
		tmp["duck_in_air"] = duck_in_air;
		tmp["night_sky"] = night_sky;
		tmp["glove_model"] = glove_model;
		tmp["glove_skin"] = glove_skin;
		save_color(water_mark_col, tmp, "water_mark_col");
		save_color(visuals_bounding_col, tmp, "visuals_bounding_col");
		save_color(visuals_bounding_team_col, tmp, "visuals_bounding_team_col");
		save_color(visuals_innacc_circle_col, tmp, "visuals_innacc_circle_col");
		save_color(visuals_backtrack_col, tmp, "visuals_backtrack_col");
		save_color(visuals_dormant_col, tmp, "visuals_dormant_col");
		save_color(visuals_dormant_col_team, tmp, "visuals_dormant_col_team");
		save_color(visuals_chams_col, tmp, "visuals_chams_col");
		save_color(visuals_chams_team_col, tmp, "visuals_chams_team_col");
		save_color(visuals_name_esp_col, tmp, "visuals_name_esp_col");
		save_color(visuals_name_esp_col_team, tmp, "visuals_name_esp_col_team");
		save_color(team_visible_col, tmp, "team_visible_col");
		save_color(team_hidden_col, tmp, "team_hidden_col");
		save_color(visuals_bounding_col_visible, tmp, "visuals_bounding_col_visible");
		save_color(visuals_bounding_col_hidden, tmp, "visuals_bounding_col_hidden");
		save_color(visuals_name_esp_col_visible, tmp, "visuals_name_esp_col_visible");
		save_color(visuals_name_esp_col_hidden, tmp, "visuals_name_esp_col_hidden");
		save_color(visuals_chams_col_xqz, tmp, "visuals_chams_col_xqz");
		save_color(visuals_chams_team_col_xqz, tmp, "visuals_chams_team_col_xqz");
		save_color(visuals_hitmarker_col, tmp, "visuals_hitmarker_col");
		save_color(nightmode_col, tmp, "nightmode_col");
		save_color(visuals_glow_player_col_visible, tmp, "visuals_glow_player_col");
		save_color(visuals_glow_player_col_hidden, tmp, "visuals_glow_player_col_hidden");
		save_color(visuals_glow_c4_col, tmp, "visuals_glow_c4_col");
		save_color(visuals_glow_chicken_col, tmp, "visuals_glow_chicken_col");
		save_color(visuals_skeleton_col_visible, tmp, "visuals_skeleton_col_visible");
		save_color(visuals_skeleton_col_hidden, tmp, "visuals_skeleton_col_hidden");
		save_color(visuals_bullet_tracer_col, tmp, "visuals_bullet_tracer_col");
		save_color(visuals_sniper_crosshair_col, tmp, "visuals_sniper_crosshair_col");
		save_color(visuals_recoil_crosshair_col, tmp, "visuals_recoil_crosshair_col");
		save_color(visuals_corner_col_hidden, tmp, "visuals_corner_col_hidden");
		save_color(visuals_corner_col_visible, tmp, "visuals_corner_col_visible");
		save_color(visuals_glow_weapon_col, tmp, "visuals_glow_weapon_col");
		save_color(visuals_fov_circle_col, tmp, "visuals_fov_circle_col");
		save_color(visuals_glow_team_col, tmp, "visuals_glow_team_col");


		json skin_tmp;
		for (auto i = 0; i < 35; i++) {
			skin_tmp[i] = skin_wheel[i].convert();
		}
		tmp["skins"] = skin_tmp;

		return tmp;
	}

	void convert(json data)
	{
		if (check("pistols", data)) legit_cfg[0].convert(data["pistols"]);
		if (check("rifle", data)) legit_cfg[1].convert(data["rifle"]);
		if (check("smg", data)) legit_cfg[2].convert(data["smg"]);
		if (check("shotgun", data)) legit_cfg[3].convert(data["shotgun"]);
		if (check("heavy", data)) legit_cfg[4].convert(data["heavy"]);
		if (check("auto", data)) legit_cfg[5].convert(data["auto"]);
		if (check("awp", data)) legit_cfg[6].convert(data["awp"]);
		if (check("scout", data)) legit_cfg[7].convert(data["scout"]);

		if (check("enable_visuals", data)) enable_visuals = data["enable_visuals"];
		if (check("enable_visuals_key", data)) enable_visuals_key = data["enable_visuals_key"];
		if (check("visuals_watermark", data)) visuals_watermark = data["visuals_watermark"];
		if (check("visuals_teammates", data)) visuals_teammates = data["visuals_teammates"];
		if (check("visuals_bounding_box", data)) visuals_bounding_box = data["visuals_bounding_box"];
		if (check("visuals_health_bars", data)) visuals_health_bars = data["visuals_health_bars"];
		if (check("visuals_inacc_circle", data)) visuals_inacc_circle = data["visuals_inacc_circle"];
		if (check("visuals_backtrack_dots", data)) visuals_backtrack_dots = data["visuals_backtrack_dots"];
		if (check("visuals_visible_only", data)) visuals_visible_only = data["visuals_visible_only"];
		if (check("visuals_bspotted", data)) visuals_bspotted = data["visuals_bspotted"];
		if (check("visuals_bomb_timer", data)) visuals_bomb_timer = data["visuals_bomb_timer"];
		if (check("visuals_dormant_esp", data)) visuals_dormant_esp = data["visuals_dormant_esp"];
		if (check("visuals_chams", data)) visuals_chams = data["visuals_chams"];
		if (check("visuals_glow_enabled", data)) visuals_glow_enabled = data["visuals_glow_enabled"];
		if (check("visuals_glow_player", data)) visuals_glow_player = data["visuals_glow_player"];
		if (check("visuals_glow_c4", data)) visuals_glow_c4 = data["visuals_glow_c4"];
		if (check("visuals_glow_chicken", data)) visuals_glow_chicken = data["visuals_glow_chicken"];
		if (check("visuals_glow_hidden_col", data)) visuals_glow_hidden_col = data["visuals_glow_hidden_col"];
		if (check("visuals_recoil_crosshair", data)) visuals_recoil_crosshair = data["visuals_recoil_crosshair"];
		if (check("visuals_sniper_crosshair", data)) visuals_sniper_crosshair = data["visuals_sniper_crosshair"];
		if (check("night_sky", data)) night_sky = data["night_sky"];
		if (check("viewmodel_changer", data)) viewmodel_changer = data["viewmodel_changer"];
		if (check("viewmodel_offset", data)) viewmodel_offset = data["viewmodel_offset"];
		if (check("visuals_corner_box", data)) visuals_corner_box = data["visuals_corner_box"];
		if (check("visuals_glow_weapon", data)) visuals_glow_weapon = data["visuals_glow_weapon"];
		if (check("disable_post_processing", data)) disable_post_processing = data["disable_post_processing"];
		if (check("screenshot_proof", data)) screenshot_proof = data["screenshot_proof"];
		if (check("visuals_fov_circle", data)) visuals_fov_circle = data["visuals_fov_circle"];
		if (check("visuals_weapon_esp", data)) visuals_weapon_esp = data["visuals_weapon_esp"];
		if (check("enable_bullet_tracers", data)) enable_bullet_tracers = data["enable_bullet_tracers"];
		if (check("bullet_tracer_expire", data)) bullet_tracer_expire = data["bullet_tracer_expire"];
		if (check("visuals_glow_team", data)) visuals_glow_team = data["visuals_glow_team"];


		// misc
		if (check("enable_misc", data)) enable_misc = data["enable_misc"];
		if (check("auto_jump", data)) auto_jump = data["auto_jump"];
		if (check("auto_jump_hitchance", data)) auto_jump_hitchance = data["auto_jump_hitchance"];
		if (check("misc_ai", data)) misc_ai = data["misc_ai"];
		if (check("misc_ai_legitfactor", data)) misc_ai_legitfactor = data["misc_ai_legitfactor"];
		if (check("misc_ai_random", data)) misc_ai_random = data["misc_ai_random"];
		if (check("misc_ai_nearest", data)) misc_ai_nearest = data["misc_ai_nearest"];
		if (check("misc_ai_defuse", data)) misc_ai_defuse = data["misc_ai_defuse"];
		if (check("misc_ai_defend", data)) misc_ai_defend = data["misc_ai_defend"];
		if (check("visuals_chams_render_team", data)) visuals_chams_render_team = data["visuals_chams_render_team"];
		if (check("skinchanger_enabled", data)) skinchanger_enabled = data["skinchanger_enabled"];
		if (check("skinchanger_selected_gun", data)) skinchanger_selected_gun = data["skinchanger_selected_gun"];
		if (check("previous_knife_index", data)) previous_knife_index = data["previous_knife_index"];
		if (check("next_knife_index", data)) next_knife_index = data["next_knife_index"];
		if (check("visuals_name_esp", data)) visuals_name_esp = data["visuals_name_esp"];
		if (check("skinchanger_apply_nxt", data)) skinchanger_apply_nxt = data["skinchanger_apply_nxt"];
		if (check("misc_legit_aa_enabled", data)) misc_legit_aa_enabled = data["misc_legit_aa_enabled"];
		if (check("misc_legit_aa_jitter", data)) misc_legit_aa_jitter = data["misc_legit_aa_jitter"];
		if (check("misc_legit_aa_edge", data)) misc_legit_aa_edge = data["misc_legit_aa_edge"];
		if (check("selected_knife_index0", data)) selected_knife_index[0] = data["selected_knife_index0"];
		if (check("selected_knife_index1", data)) selected_knife_index[1] = data["selected_knife_index1"];
		if (check("selected_gun_index", data)) selected_gun_index = data["selected_gun_index"];
		if (check("visuals_backtrack_style", data)) visuals_backtrack_style = data["visuals_backtrack_style"];
		if (check("anti_triggerbot", data)) anti_triggerbot = data["anti_triggerbot"];
		if (check("misc_clantag_changer", data)) misc_clantag_changer = data["misc_clantag_changer"];
		if (check("misc_chat_spammer", data)) misc_chat_spammer = data["misc_chat_spammer"];
		if (check("misc_slidewalk", data)) misc_slidewalk = data["misc_slidewalk"];
		if (check("anti_triggerbot_key", data)) anti_triggerbot_key = data["anti_triggerbot_key"];
		if (check("misc_legit_aa_resolver", data)) misc_legit_aa_resolver = data["misc_legit_aa_resolver"];
		if (check("misc_ai_rotationspeed", data)) misc_ai_rotationspeed = data["misc_ai_rotationspeed"];
		if (check("misc_ai_aimspeed", data)) misc_ai_aimspeed = data["misc_ai_aimspeed"];
		if (check("visuals_nightmode", data)) visuals_nightmode = data["visuals_nightmode"];
		if (check("visuals_chams_xqz", data)) visuals_chams_xqz = data["visuals_chams_xqz"];
		if (check("visuals_hitmarkers", data)) visuals_hitmarkers = data["visuals_hitmarkers"];
		if (check("hitmarker_sound", data)) hitmarker_sound = data["hitmarker_sound"];
		if (check("rank_reveal", data)) rank_reveal = data["rank_reveal"];
		if (check("misc_autoAccept", data)) misc_autoAccept = data["misc_autoAccept"];
		if (check("minecraft_pickaxe", data)) minecraft_pickaxe = data["minecraft_pickaxe"];
		if (check("reina_model", data)) anime_model = data["reina_model"];
		if (check("auto_strafe", data)) auto_strafe = data["auto_strafe"];
		if (check("thirdperson", data)) thirdperson = data["thirdperson"];
		if (check("killsay", data)) killsay = data["killsay"];
		if (check("wireframe_smoke", data)) wireframe_smoke = data["wireframe_smoke"];
		if (check("visuals_skeleton", data)) visuals_skeleton = data["visuals_skeleton"];
		if (check("edge_jump", data)) edge_jump = data["edge_jump"];
		if (check("edge_jump_key", data)) edge_jump_key = data["edge_jump_key"];
		if (check("duck_in_air", data)) duck_in_air = data["duck_in_air"];
		if (check("glove_model", data)) glove_model = data["glove_model"];
		if (check("glove_skin", data)) glove_skin = data["glove_skin"];

		// colors

		if (check_color("water_mark_col", data)) read_color(water_mark_col, data, "water_mark_col");
		if (check_color("visuals_bounding_col", data)) read_color(visuals_bounding_col, data, "visuals_bounding_col");
		if (check_color("visuals_bounding_team_col", data)) read_color(visuals_bounding_team_col, data, "visuals_bounding_team_col");
		if (check_color("visuals_innacc_circle_col", data)) read_color(visuals_innacc_circle_col, data, "visuals_innacc_circle_col");
		if (check_color("visuals_backtrack_col", data)) read_color(visuals_backtrack_col, data, "visuals_backtrack_col");
		if (check_color("visuals_dormant_col", data)) read_color(visuals_dormant_col, data, "visuals_dormant_col");
		if (check_color("visuals_dormant_col_team", data)) read_color(visuals_dormant_col_team, data, "visuals_dormant_col_team");
		if (check_color("visuals_chams_col", data)) read_color(visuals_chams_col, data, "visuals_chams_col");
		if (check_color("visuals_chams_team_col", data)) read_color(visuals_chams_team_col, data, "visuals_chams_team_col");
		if (check_color("visuals_name_esp_col", data)) read_color(visuals_name_esp_col, data, "visuals_name_esp_col");
		if (check_color("visuals_name_esp_col_team", data)) read_color(visuals_name_esp_col_team, data, "visuals_name_esp_col_team");
		if (check_color("team_visible_col", data)) read_color(team_visible_col, data, "team_visible_col");
		if (check_color("team_hidden_col", data)) read_color(team_hidden_col, data, "team_hidden_col");
		if (check_color("visuals_bounding_col_visible", data)) read_color(visuals_bounding_col_visible, data, "visuals_bounding_col_visible");
		if (check_color("visuals_bounding_col_hidden", data)) read_color(visuals_bounding_col_hidden, data, "visuals_bounding_col_hidden");
		if (check_color("visuals_name_esp_col_visible", data)) read_color(visuals_name_esp_col_visible, data, "visuals_name_esp_col_visible");
		if (check_color("visuals_name_esp_col_hidden", data)) read_color(visuals_name_esp_col_hidden, data, "visuals_name_esp_col_hidden");
		if (check_color("visuals_chams_col_xqz", data)) read_color(visuals_chams_col_xqz, data, "visuals_chams_col_xqz");
		if (check_color("visuals_chams_team_col_xqz", data)) read_color(visuals_chams_team_col_xqz, data, "visuals_chams_team_col_xqz");
		if (check_color("visuals_hitmarker_col", data)) read_color(visuals_hitmarker_col, data, "visuals_hitmarker_col");
		if (check_color("nightmode_col", data)) read_color(nightmode_col, data, "nightmode_col");
		if (check_color("visuals_glow_player_col", data)) read_color(visuals_glow_player_col_visible, data, "visuals_glow_player_col");
		if (check_color("visuals_glow_player_col_hidden", data)) read_color(visuals_glow_player_col_hidden, data, "visuals_glow_player_col_hidden");
		if (check_color("visuals_glow_c4_col", data)) read_color(visuals_glow_c4_col, data, "visuals_glow_c4_col");
		if (check_color("visuals_glow_chicken_col", data)) read_color(visuals_glow_chicken_col, data, "visuals_glow_chicken_col");
		if (check_color("visuals_skeleton_col_visible", data)) read_color(visuals_skeleton_col_visible, data, "visuals_skeleton_col_visible");
		if (check_color("visuals_skeleton_col_hidden", data)) read_color(visuals_skeleton_col_hidden, data, "visuals_skeleton_col_hidden");
		if (check_color("visuals_bullet_tracer_col", data)) read_color(visuals_bullet_tracer_col, data, "visuals_bullet_tracer_col");
		if (check_color("visuals_sniper_crosshair_col", data)) read_color(visuals_sniper_crosshair_col, data, "visuals_sniper_crosshair_col");
		if (check_color("visuals_recoil_crosshair_col", data)) read_color(visuals_recoil_crosshair_col, data, "visuals_recoil_crosshair_col");
		if (check_color("visuals_corner_col_hidden", data)) read_color(visuals_corner_col_hidden, data, "visuals_corner_col_hidden");
		if (check_color("visuals_corner_col_visible", data)) read_color(visuals_corner_col_visible, data, "visuals_corner_col_visible");
		if (check_color("visuals_glow_weapon_col", data)) read_color(visuals_glow_weapon_col, data, "visuals_glow_weapon_col");
		if (check_color("visuals_fov_circle_col", data)) read_color(visuals_fov_circle_col, data, "visuals_fov_circle_col");
		if (check_color("visuals_glow_team_col", data)) read_color(visuals_glow_team_col, data, "visuals_glow_team_col");
		

		json skin_tmp = data["skins"];

		for (auto i = 0; i < 35; i++) {
			skin_wheel[i].convert(skin_tmp[i]);
		}

	}

	void save_cfg(std::ostream& file)
	{
		try {
			file << convert();
		}
		catch (...) {

		}
	}

	void load_cfg(std::istream& file)
	{
		try {
			json main;
			main << file;
			convert(main);

		}
		catch (...) {

		}

	}


};

extern c_weebwarecfg g_weebwarecfg;
#endif