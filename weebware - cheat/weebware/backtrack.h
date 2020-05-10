#pragma once
#include "Header.h"


struct BacktrackRecord
{
	c_base_entity* entity;
	Vector head;
	Vector origin;
};

struct BacktrackTick
{
	int tickcount;
	std::vector<BacktrackRecord> records;
};


class c_backtrack {
	c_base_entity* m_local;
	std::vector<BacktrackTick> ticks;
	Vector prevOrig;

public:
	void init(c_usercmd* cmd);
	void register_tick(c_usercmd* cmd);
};