#pragma once
#include "Header.h"

class c_create_move
{
public:
	c_base_entity* local;
	void create_move(c_usercmd* cmd, bool& sendPackets);
	void correct_movement(Vector old_view_angles, c_usercmd* cmd);
	void run_legitAA(c_usercmd* cmd, bool send_packets);
	void runClanTag();
	void chat_spam();

private:
	void auto_jump(c_usercmd* cmd);
	void run_fake(c_usercmd* cmd, bool &send_packet);
	bool anti_trigger(c_usercmd* cmd, bool& send_packets);
	bool is_visible(c_base_entity* target);
};

extern c_create_move g_create_move;
