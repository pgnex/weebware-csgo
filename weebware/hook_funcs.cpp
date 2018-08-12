#include "Header.h"
#include "shared.h"
#include "Polyhook\PolyHook\PolyHook.hpp"

c_hooking g_hooking;

PLH::VEHHook* VEH_PAINT;
PLH::VEHHook* VEH_CM;
PLH::VEHHook* VEH_RESET;
PLH::VEHHook* VEH_FSN;
PLH::VEHHook* VEH_PRESENT;
PLH::VEHHook* VEH_ENDSCENE;
PLH::VEHHook* VEH_DME;

void __stdcall hk_paint_traverse(unsigned int v, bool f, bool a)
{
	auto protecc = VEH_PAINT->GetProtectionObject();

	hook_functions::paint_traverse(v, f, a);
}

bool __stdcall hk_clientmode_cm(float input_sample_time, c_usercmd* cmd)
{
	auto protecc = VEH_CM->GetProtectionObject();

	return hook_functions::clientmode_cm(input_sample_time, cmd);
}

long __stdcall hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param)
{
	auto protecc = VEH_RESET->GetProtectionObject();

	return hook_functions::reset(device, presentation_param);
}

long __stdcall hk_endscene(IDirect3DDevice9* device)
{
	auto protecc = VEH_ENDSCENE->GetProtectionObject();

	return hook_functions::end_scene(device);
}

void __fastcall hk_draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
	auto protecc = VEH_DME->GetProtectionObject();

	hook_functions::draw_model_execute(thisptr, edx, ctx, state, pInfo, pCustomBoneToWorld);
}

void c_hooking::hook_all_functions()
{
	VEH_PAINT = new PLH::VEHHook();
	VEH_PAINT->SetupHook((BYTE*)(*reinterpret_cast<uintptr_t**>(g_weebware.g_panel))[41], (BYTE*)&hk_paint_traverse, PLH::VEHHook::VEHMethod::INT3_BP);
	VEH_PAINT->Hook();
	o_painttraverse = VEH_PAINT->GetOriginal<pt_t>();

	VEH_CM = new PLH::VEHHook();
	VEH_CM->SetupHook((BYTE*)(*reinterpret_cast<uintptr_t**>(g_weebware.g_client_mode))[24], (BYTE*)&hk_clientmode_cm, PLH::VEHHook::VEHMethod::INT3_BP);
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

#if 0

	original_present = **reinterpret_cast<decltype(&original_present)*>(g_weebware.g_present_address);
	**reinterpret_cast<void***>(g_weebware.g_present_address) = reinterpret_cast<void*>(&hook_functions::hk_present); // (*(uintptr_t**)this)[0]

	original_reset = **reinterpret_cast<decltype(&original_reset)*>(g_weebware.g_reset_address);
	**reinterpret_cast<void***>(g_weebware.g_reset_address) = reinterpret_cast<void*>(&hk_reset);

#endif

	VEH_RESET = new PLH::VEHHook();
	VEH_RESET->SetupHook((BYTE*)(*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[16], (BYTE*)&hk_reset, PLH::VEHHook::VEHMethod::INT3_BP);
	VEH_RESET->Hook();
	o_reset = VEH_RESET->GetOriginal<fn_reset>();

	VEH_ENDSCENE = new PLH::VEHHook();
	VEH_ENDSCENE->SetupHook((BYTE*)(*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[42], (BYTE*)&hk_endscene, PLH::VEHHook::VEHMethod::INT3_BP);
	VEH_ENDSCENE->Hook();
	o_endscene = VEH_ENDSCENE->GetOriginal<fn_endscene>();
	
	VEH_DME = new PLH::VEHHook();
	VEH_DME->SetupHook((BYTE*)(*reinterpret_cast<uintptr_t**>(g_weebware.g_model_render))[21], (BYTE*)&hk_draw_model_execute, PLH::VEHHook::VEHMethod::INT3_BP);
	VEH_DME->Hook();
	o_dme = VEH_DME->GetOriginal<fn_dme>();

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
	VEH_ENDSCENE->UnHook();
	VEH_DME->UnHook();
	// 	tramp_fsn->UnHook();
	SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)g_weebware.old_window_proc);
	g_vars.g_unload.set(1.0f);
}