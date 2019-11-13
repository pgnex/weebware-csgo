#include "Ragebot.h"

bool c_ragebot::Run(c_usercmd* cmd) {

	// reset general stuff
	if (get_epoch() - best_target_found > 100) { // only reset target if not valid for 100 ms
		best_target_index = -1;
		best_target_vector = { };
	}

	return false;
}

long c_ragebot::get_epoch()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}