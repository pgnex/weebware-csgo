#pragma once
#include "Header.h"
#include "shared.h"

#include <Windows.h>
#include <vector>

class c_knifehook {
public:
	void knife_animation() noexcept;
	void force_update();
};

extern c_knifehook knife_hook;
