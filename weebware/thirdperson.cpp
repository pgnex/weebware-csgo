#include "Header.h"
#include "thirdperson.h"

static bool done = false;
void thirdperson::fsn(c_base_entity* local) {
	if (!done && g_weebwarecfg.thirdperson) {
		if (local && local->is_valid_player()) {
			c_convar* tp = g_weebware.g_convars->find_cvar("thirdperson");
			tp->SetValue("1");
			done = true;
		}
	}
	else if (!g_weebwarecfg.thirdperson && done) {
		if (local && local->is_valid_player()) {
			c_convar* fp = g_weebware.g_convars->find_cvar("firstperson");
			fp->SetValue("1");
			done = false;
		}
	}
}