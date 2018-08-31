#pragma once
#include "Header.h"

class c_create_move
{
public:
	c_base_entity* local;
	bool send_packet;
	void create_move(c_usercmd* cmd, bool& sendPackets);
	void correct_movement(Vector old_view_angles, c_usercmd* cmd);
private:
	void auto_jump(c_usercmd* cmd);
	void run_fake(c_usercmd* cmd, bool &send_packet);
	void run_legitAA(c_usercmd* cmd, bool send_packets);
	bool anti_trigger(c_usercmd* cmd, bool& send_packets);

};

extern c_create_move g_create_move;
