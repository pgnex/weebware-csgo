#include "frame_stage.h"
#include "Header.h"
#include "shared.h"

c_frame_stage_notify g_frame_stage_notify;

#if 1
void __stdcall hook_functions::hk_frame_stage_notify(clientframestage_t curStage)
{
	if (!(g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game()))
	{
		return g_hooking.o_fsn(g_weebware.g_client,curStage);
	}

	g_frame_stage_notify.local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local()); 
	
	if (!g_frame_stage_notify.local)
	{
		return g_hooking.o_fsn(g_weebware.g_client, curStage);
	}

	if (curStage == clientframestage_t::frame_render_start)
	{
		g_frame_stage_notify.pvs_fix();
	}

	g_hooking.o_fsn(g_weebware.g_client, curStage);
}
#endif

void c_frame_stage_notify::pvs_fix()
{
	for (int i = 1; i <= 99; i++)
	{
		if (i == local->index)
			continue;

		c_base_entity* ent = g_weebware.g_entlist->getcliententity(i);

		if (!ent->is_valid_player())
			continue;

		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ent) + 0xA30) = g_weebware.g_global_vars->framecount;
		*reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ent) + 0xA28) = NULL;
	}
}

void update_skinwheel()
{
	// The skin wheel will be a selection of skins saved

}

void run_skinchanger()
{
	// 
	// Get Skin ID

}
