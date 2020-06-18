#pragma once
#include "Header.h"

class LegitAntiAim
{
public:
	void Run(c_usercmd* cmd);

private:
	int last_moving(c_usercmd* cmd);
};

extern LegitAntiAim g_LegitAntiAim;