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

void __stdcall hk_paint_traverse(unsigned int v, bool f, bool a)
{
	auto protecc = VEH_PAINT->getProtectionObject();

	hook_functions::paint_traverse(v, f, a);
}

bool __stdcall hk_clientmode_cm(float input_sample_time, c_usercmd* cmd)
{
	auto protecc = VEH_CM->getProtectionObject();

	return hook_functions::clientmode_cm(input_sample_time, cmd);
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

	hook_functions::draw_model_execute(thisptr, edx, ctx, state, pInfo, pCustomBoneToWorld);
}

void c_hooking::hook_all_functions()
{
	auto paint_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_panel))[41];
	VEH_PAINT = new PLH::BreakPointHook((const char*)paint_addr, (const char*)&hk_paint_traverse);
	VEH_PAINT->hook();
	o_painttraverse = reinterpret_cast<pt_t>(paint_addr);

	auto cm_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_client_mode))[24];
	VEH_CM = new PLH::BreakPointHook((const char*)cm_addr, (const char*)&hk_clientmode_cm);
	VEH_CM->hook();
	o_createmove = reinterpret_cast<fn_createmove>(cm_addr);

#if 0

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
	o_reset = reinterpret_cast<fn_reset>(reset_addr);

	auto end_scene_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[42];
	VEH_ENDSCENE = new PLH::BreakPointHook((const char*)end_scene_addr, (const char*)&hk_endscene);
	VEH_ENDSCENE->hook();
	o_endscene = reinterpret_cast<fn_endscene>(end_scene_addr);

	auto dme_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_model_render))[21];
	VEH_DME = new PLH::BreakPointHook((const char*)dme_addr, (const char*)&hk_draw_model_execute);
	VEH_DME->hook();
	o_dme = reinterpret_cast<fn_dme>(dme_addr); 

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

void c_hooking::unhook_all_functions()
{
	g_weebware.g_engine->client_cmd_unrestricted("cl_mouseenable 1", NULL);
	g_weebware.menu_opened = false;

	VEH_PAINT->unHook();
	VEH_CM->unHook();
	VEH_RESET->unHook();
	VEH_ENDSCENE->unHook();
	VEH_DME->unHook();
	// 	tramp_fsn->UnHook();
	SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)g_weebware.old_window_proc);
	g_vars.g_unload.set(1.0f);
}