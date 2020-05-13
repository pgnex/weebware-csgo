#pragma once
#include "Header.h"

class c_create_move
{
public:
	c_base_entity* local;
	void correct_movement(Vector old_view_angles, c_usercmd* cmd);
	void fake_lag(c_usercmd* cmd, bool send_packets);
	void chat_spam();
	void rank_reveal();
	void skybox_changer();
	bool is_sky_set = false;
	void edge_jump_pre_prediction(c_usercmd* cmd);
	void edge_jump_post_prediction(c_usercmd* cmd);
	void auto_jump(c_usercmd* cmd);
	void run_fake(c_usercmd* cmd, bool &send_packet);
	bool anti_trigger(c_usercmd* cmd, bool& send_packets);
	void auto_strafe(c_usercmd* cmd);
	void slidewalk(c_usercmd* cmd);
	void disable_post_processing();
	void draw_grenade_trajectory();
	bool grenade_traj_disabled = false;
	void clamp_angles(c_usercmd* cmd, Vector original_angles, bool& sendpacket);
	void auto_pistol(c_usercmd* cmd);
	bool is_ground();
	void auto_jumpbug(c_usercmd* cmd);
	void rainbow_name();
	void block_bot(c_usercmd* cmd);
	void auto_defuse(c_usercmd* cmd);
	void no_crouch_cooldown(c_usercmd* cmd);
	void anti_afk(c_usercmd* cmd);

	std::vector<std::string> cspam_weebware = {
	"weebware.net - premium cheating software, get weebware!",
	"weebware.net - cheating is for cool kids, get weebware!",
	"weebware.net - same price as an esea sub, lol, get weebware!",
	"weebware.net - would u like some sauce with ur pasta?",
	"weebware.net - no sir, i've never heard of ayyware b4",
	"weebware.net - you pay for that baim?",
	"weebware.net - fanta.trashed on",
	"weebware.net - does it come with junkcode",
	"weebware.net - handin out L's",
	"weebware.net - where your security matters."
	};

	QAngle real_angle;
};

extern c_create_move g_create_move;
