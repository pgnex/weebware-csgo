#include "Header.h"

void overrideview::thirdperson(void* thisptr, int, view_setup_t* vsView) {

	if (!g_weebware.g_engine->is_connected() || !g_weebware.g_engine->is_in_game())
		return;

	// get local
	c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
	if (!local)
		return;

}
