#include "Header.h"
#include "shared.h"

// #include "Polyhook\PolyHook\PolyHook.hpp"
// Update 
#include "PolyHook2/PolyHook_2_0/headers/IHook.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Misc.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Exceptions/BreakPointHook.hpp"

c_hooking g_hooking;

PLH::BreakPointHook* VEH_PAINT;
PLH::BreakPointHook* VEH_CM;
PLH::BreakPointHook* VEH_RESET;
PLH::BreakPointHook* VEH_FSN;
PLH::BreakPointHook* VEH_PRESENT;
PLH::BreakPointHook* VEH_ENDSCENE;
PLH::BreakPointHook* VEH_DME;
PLH::BreakPointHook* VEH_CURSORLOCK;

void __stdcall hk_unlock_cursor()
{
	auto protecc = VEH_CURSORLOCK->getProtectionObject();

	if (g_weebware.menu_opened) {

		g_weebware.g_surface->unlockcursor();

	}
	else {
		g_hooking.o_cursor(g_weebware.g_surface);
	}
}

void __stdcall hk_paint_traverse(unsigned int v, bool f, bool a)
{
	auto protecc = VEH_PAINT->getProtectionObject();
	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		hook_functions::paint_traverse(v, f, a);
	else
		g_hooking.o_painttraverse(g_weebware.g_panel, v, f, a);
}

bool __stdcall hk_clientmode_cm(float input_sample_time, c_usercmd* cmd)
{
	auto protecc = VEH_CM->getProtectionObject();

	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		return hook_functions::clientmode_cm(input_sample_time, cmd);
	else
		return g_hooking.o_createmove(g_weebware.g_client_mode, input_sample_time, cmd);
}

long __stdcall hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param)
{
	auto protecc = VEH_RESET->getProtectionObject();

	return hook_functions::reset(device, presentation_param);
}

long __stdcall hk_endscene(IDirect3DDevice9* device)
{
	auto protecc = VEH_ENDSCENE->getProtectionObject();

	return hook_functions::end_scene(device);
}

void __fastcall hk_draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
	auto protecc = VEH_DME->getProtectionObject();

	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		hook_functions::draw_model_execute(thisptr, edx, ctx, state, pInfo, pCustomBoneToWorld);
	else
		g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
}

void __stdcall hk_frame_stage_notify(clientframestage_t curStage)
{
	auto protecc = VEH_FSN->getProtectionObject();

	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		hook_functions::frame_stage_notify(curStage);
	else
		g_hooking.o_fsn(curStage);
}

namespace knife_changer {

	RecvVarProxyFn original_sequence;

	enum ESequence
	{
		SEQUENCE_DEFAULT_DRAW = 0,
		SEQUENCE_DEFAULT_IDLE1 = 1,
		SEQUENCE_DEFAULT_IDLE2 = 2,
		SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
		SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
		SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
		SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
		SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
		SEQUENCE_DEFAULT_LOOKAT01 = 12,

		SEQUENCE_BUTTERFLY_DRAW = 0,
		SEQUENCE_BUTTERFLY_DRAW2 = 1,
		SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
		SEQUENCE_BUTTERFLY_LOOKAT03 = 15,

		SEQUENCE_FALCHION_IDLE1 = 1,
		SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
		SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
		SEQUENCE_FALCHION_LOOKAT01 = 12,
		SEQUENCE_FALCHION_LOOKAT02 = 13,

		SEQUENCE_DAGGERS_IDLE1 = 1,
		SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
		SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
		SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
		SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,

		SEQUENCE_BOWIE_IDLE1 = 1,
	};

	static auto random_sequence(const int low, const int high) -> int
	{
		return rand() % (high - low + 1) + low;
	}

	void animation_fix(const c_rec_proxy_data* pDataConst, void* pStruct, void* pOut)
	{
		c_rec_proxy_data* pData = const_cast<c_rec_proxy_data*>(pDataConst);

		if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		{
			auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

			if (local) {

				auto weapon = local->m_pActiveWeapon();

				if (weapon && local->m_pActiveWeapon()->is_knife()) {

					int current_seq = pData->m_Value.m_Int;

					if (g_weebwarecfg.selected_knife.weapon_index == 515) {

						switch (current_seq)
						{
						case SEQUENCE_DEFAULT_DRAW:
							current_seq = random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
						case SEQUENCE_DEFAULT_LOOKAT01:
							current_seq = random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
						default:
							current_seq++;
						}

					}
					if (g_weebwarecfg.selected_knife.weapon_index == 512) {

						switch (current_seq)
						{
						case SEQUENCE_DEFAULT_IDLE2:
							current_seq = SEQUENCE_FALCHION_IDLE1;
						case SEQUENCE_DEFAULT_HEAVY_MISS1:
							current_seq = random_sequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
						case SEQUENCE_DEFAULT_LOOKAT01:
							current_seq = random_sequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
						case SEQUENCE_DEFAULT_DRAW:
						case SEQUENCE_DEFAULT_IDLE1:
							current_seq = current_seq;
						default:
							current_seq--;
						}
					}
					if (g_weebwarecfg.selected_knife.weapon_index == 516) {

						switch (current_seq)
						{
						case SEQUENCE_DEFAULT_IDLE2:
							current_seq = SEQUENCE_DAGGERS_IDLE1;
						case SEQUENCE_DEFAULT_LIGHT_MISS1:
						case SEQUENCE_DEFAULT_LIGHT_MISS2:
							current_seq = random_sequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
						case SEQUENCE_DEFAULT_HEAVY_MISS1:
							current_seq = random_sequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
						case SEQUENCE_DEFAULT_HEAVY_HIT1:
						case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
						case SEQUENCE_DEFAULT_LOOKAT01:
							current_seq += 3;
						case SEQUENCE_DEFAULT_DRAW:
						case SEQUENCE_DEFAULT_IDLE1:
							current_seq = current_seq;
						default:
							current_seq += 2;
						}

					}
					if (g_weebwarecfg.selected_knife.weapon_index == 514) {
						switch (current_seq)
						{
						case SEQUENCE_DEFAULT_DRAW:
						case SEQUENCE_DEFAULT_IDLE1:
							current_seq = current_seq;
						case SEQUENCE_DEFAULT_IDLE2:
							current_seq = SEQUENCE_BOWIE_IDLE1;
						default:
							current_seq--;
						}
					}
					if (g_weebwarecfg.selected_knife.weapon_index == 519) {

						switch (current_seq)
						{
						case SEQUENCE_DEFAULT_DRAW:
							current_seq = random_sequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
						case SEQUENCE_DEFAULT_LOOKAT01:
							current_seq = random_sequence(SEQUENCE_BUTTERFLY_LOOKAT01, 14);
						default:
							current_seq++;
						}

					}
					if (g_weebwarecfg.selected_knife.weapon_index == 522) {
						switch (current_seq)
						{
						case SEQUENCE_DEFAULT_LOOKAT01:
							current_seq = random_sequence(12, 13);
						}
					}

					if (g_weebwarecfg.selected_knife.weapon_index == 523) {
						switch (current_seq)
						{
						case SEQUENCE_DEFAULT_LOOKAT01:
							current_seq = random_sequence(14, 15);
						}
					}

					pData->m_Value.m_Int = current_seq;

				}
			}
		}

		original_sequence(pData, pStruct, pOut);
	}


	void apply_proxyhooks()
	{

		for (client_class* p_class = g_weebware.g_client->get_all_classes(); p_class; p_class = p_class->m_pNext)
		{
			auto class_name = p_class->m_precvtable->m_pNetTableName;

			if (!strcmp(p_class->m_networkedname, "CBaseViewModel")) {

				for (auto i = 0; i < p_class->m_precvtable->m_nProps; i++) {

					auto p_prop = &p_class->m_precvtable->m_pProps[i];

					auto name = p_prop->m_pVarName;

					// animations
					if (!strcmp(name, "m_nSequence"))
					{
						original_sequence = reinterpret_cast<RecvVarProxyFn>(p_prop->m_ProxyFn);
						p_prop->m_ProxyFn = reinterpret_cast<RecvVarProxyFn>(animation_fix);

						break;
					}
				}
				break;
			}
		}
	}

	void remove_proxyhooks()
	{
		auto p_class = g_weebware.g_client->get_all_classes();

		while (p_class) {

			auto class_name = p_class->m_precvtable->m_pNetTableName;

			if (!strcmp(class_name, "DT_BaseViewModel")) {

				for (auto i = 0; i < p_class->m_precvtable->m_nProps; i++) {

					auto p_prop = &(p_class->m_precvtable->m_pProps[i]);

					auto name = p_prop->m_pVarName;

					// animations
					if (!strcmp(name, "m_nSequence"))
					{
						p_prop->m_ProxyFn = reinterpret_cast<RecvVarProxyFn>(original_sequence);
					}

				}

			}

			p_class = p_class->m_pNext;
		}
	}


}
void c_hooking::hook_all_functions()
{
	g_weebware.old_window_proc = (WNDPROC)SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)hook_functions::hk_window_proc);

	auto paint_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_panel))[41];
	VEH_PAINT = new PLH::BreakPointHook((const char*)paint_addr, (const char*)&hk_paint_traverse);
	VEH_PAINT->hook();
	o_painttraverse = reinterpret_cast<decltype(o_painttraverse)>(paint_addr);


	auto cursor_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_surface))[67];
	VEH_CURSORLOCK = new PLH::BreakPointHook((const char*)cursor_addr, (const char*)&hk_unlock_cursor);
	VEH_CURSORLOCK->hook();
	o_cursor = reinterpret_cast<decltype(o_cursor)>(cursor_addr);

	auto cm_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_client_mode))[24];
	VEH_CM = new PLH::BreakPointHook((const char*)cm_addr, (const char*)&hk_clientmode_cm);
	VEH_CM->hook();
	o_createmove = reinterpret_cast<decltype(o_createmove)>(cm_addr);


#define StreamProof 0
#if StreamProof

	//vmt_render_view = vmt_manager(reinterpret_cast<uintptr_t*>(g_weebware.g_render_view));
	// vmt_render_view.hook_m((*(unsigned long**)this)[function_by_count::scene_end], 9);

	// https://www.unknowncheats.me/forum/direct3d/66594-d3d9-vtables.html
	// vmt_direct_x.hook_m((*(unsigned long**)this)[function_by_count::end_scene], 42);
	// vmt_direct_x.hook_m((*(unsigned long**)this)[function_by_count::present], 17);
	// DWORD* vtable = (DWORD*)g_entry.pattern_scan("gameoverlayrenderer.dll", "8B 00 8B 30 FF 76 08") + 2; // Adress to   v12.lpVtbl = (IDirect3DDevice9Vtbl *)**a1;
	// sub_1006C670+112    02C                 mov     eax, [eax]
	// sub_1006C670+114    02C                 mov     esi, [eax]


	original_present = **reinterpret_cast<decltype(&original_present)*>(g_weebware.g_present_address);
	**reinterpret_cast<void***>(g_weebware.g_present_address) = reinterpret_cast<void*>(&hook_functions::hk_present); // (*(uintptr_t**)this)[0]

	original_reset = **reinterpret_cast<decltype(&original_reset)*>(g_weebware.g_reset_address);
	**reinterpret_cast<void***>(g_weebware.g_reset_address) = reinterpret_cast<void*>(&hk_reset);

#endif

	auto reset_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[16];
	VEH_RESET = new PLH::BreakPointHook((const char*)reset_addr, (const char*)&hk_reset);
	VEH_RESET->hook();
	o_reset = reinterpret_cast<decltype(o_reset)>(reset_addr);

	auto end_scene_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[42];
	VEH_ENDSCENE = new PLH::BreakPointHook((const char*)end_scene_addr, (const char*)&hk_endscene);
	VEH_ENDSCENE->hook();
	o_endscene = reinterpret_cast<decltype(o_endscene)>(end_scene_addr);

	auto dme_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_model_render))[21];
	VEH_DME = new PLH::BreakPointHook((const char*)dme_addr, (const char*)&hk_draw_model_execute);
	VEH_DME->hook();
	o_dme = reinterpret_cast<decltype(o_dme)>(dme_addr);

	auto fsn_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_client))[37];
	VEH_FSN = new PLH::BreakPointHook((const char*)fsn_addr, (const char*)&hk_frame_stage_notify);
	VEH_FSN->hook();
	o_fsn = reinterpret_cast<decltype(o_fsn)>(fsn_addr);

	knife_changer::apply_proxyhooks();
}

void c_hooking::unhook_all_functions()
{
	g_weebware.menu_opened = false;

	VEH_PAINT->unHook();
	VEH_CM->unHook();
	VEH_RESET->unHook();
	VEH_ENDSCENE->unHook();
	VEH_DME->unHook();
	VEH_FSN->unHook();
	VEH_CURSORLOCK->unHook();
	SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)g_weebware.old_window_proc);
	knife_changer::remove_proxyhooks();
	g_vars.g_unload.set(1.0f);
}