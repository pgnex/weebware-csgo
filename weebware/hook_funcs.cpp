#include "Header.h"
#include "shared.h"
#include "Polyhook\PolyHook\PolyHook.hpp"

c_hooking g_hooking;

PLH::VEHHook* VEH_PAINT;
PLH::VEHHook* VEH_CM;
PLH::VEHHook* VEH_RESET;
PLH::VEHHook* VEH_FSN;
PLH::VEHHook* VEH_PRESENT;


void c_hooking::hook_all_functions()
{
	VEH_PAINT = new PLH::VEHHook();
	VEH_PAINT->SetupHook((BYTE*)(*reinterpret_cast<uintptr_t**>(g_weebware.g_panel))[41], (BYTE*)&hook_functions::hk_paint_traverse, PLH::VEHHook::VEHMethod::HARDWARE_BP);
	VEH_PAINT->Hook();
	o_painttraverse = VEH_PAINT->GetOriginal<pt_t>();

	// tramp_paint = new c_trampolines(, &hook_functions::hk_paint_traverse, create_original(o_painttraverse));

	// tramp_paint = new c_trampolines((*reinterpret_cast<LPVOID**>(g_weebware.g_panel))[41], &hook_functions::hk_paint_traverse, create_original(o_painttraverse));

	VEH_CM = new PLH::VEHHook();
	VEH_CM->SetupHook((BYTE*)(*reinterpret_cast<uintptr_t**>(g_weebware.g_client_mode))[24], (BYTE*)&hook_functions::hk_clientmode_cm, PLH::VEHHook::VEHMethod::HARDWARE_BP);
	VEH_CM->Hook();
	o_createmove = VEH_CM->GetOriginal<fn_createmove>();

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

	VEH_RESET = new PLH::VEHHook();
	VEH_RESET->SetupHook((BYTE*)(*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[16], (BYTE*)&hook_functions::hk_reset, PLH::VEHHook::VEHMethod::HARDWARE_BP);
	VEH_RESET->Hook();
	o_reset = VEH_RESET->GetOriginal<fn_reset>();

#if 0
	tramp_fsn = new PLH::X86Detour();
	tramp_fsn->SetupHook((BYTE*)(*reinterpret_cast<uintptr_t**>(g_weebware.g_client))[36], (BYTE*)&hook_functions::hk_frame_stage_notify);
	tramp_fsn->Hook();
	o_fsn = tramp_fsn->GetOriginal<fn_fsn>();
#endif

	g_weebware.old_window_proc = (WNDPROC)SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)hook_functions::hk_window_proc);

	// vmt_client = vmt_manager(reinterpret_cast<uintptr_t*>(g_weebware.g_client));
	// vmt_client.hook_m((*(uintptr_t**)this)[function_by_count::frame_stage_notify], 36);
}

/*
// Yea i enjoy crashes too...
// **reinterpret_cast<void***>(g_entry.g_present_address) = reinterpret_cast<void*>((*reinterpret_cast<void**>(g_entry.g_present_address)));

	Maybe attempt to simulate unhooked steam overlay?
	int __stdcall UnhookingFunction(HMODULE hLibModule, int a2, int a3)
	55 8B EC 64 A1 ? ? ? ? 6A FF 68 51 8F 0C 10
*/

void c_hooking::unhook_all_functions()
{
	g_weebware.g_engine->client_cmd_unrestricted("cl_mouseenable 1", NULL);
	g_weebware.menu_opened = false;

	VEH_PAINT->UnHook();
	VEH_CM->UnHook();
	VEH_RESET->UnHook();
	// 	tramp_fsn->UnHook();
	SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)g_weebware.old_window_proc);
	g_vars.g_unload.set(1.0f);
}