#pragma once
#include "Header.h"

class c_create_move
{
public:
	c_base_entity* local;
	void create_move(c_usercmd* cmd, bool& sendPackets);
	void correct_movement(Vector old_view_angles, c_usercmd* cmd);
	void run_legitAA(c_usercmd* cmd, bool send_packets);
	void chat_spam();
	void rank_reveal();
	void skybox_changer();
	void edge_jump_pre_prediction(c_usercmd* cmd);
	void edge_jump_post_prediction(c_usercmd* cmd);
	void auto_jump(c_usercmd* cmd);
	void run_fake(c_usercmd* cmd, bool &send_packet);
	bool anti_trigger(c_usercmd* cmd, bool& send_packets);
	bool is_visible(c_base_entity* target);
	void auto_strafe(c_usercmd* cmd);
	void auto_queue();
	QAngle real_angle;
};

extern c_create_move g_create_move;
