#pragma once

class c_nightmode
{
public:
	void run();
	void apply();
	void remove();
	bool done = false;
};

extern c_nightmode g_nightmode;
