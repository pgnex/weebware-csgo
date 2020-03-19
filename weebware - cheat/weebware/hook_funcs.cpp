#include "Header.h"
#include "shared.h"
#include "gui.h"
#include "key_values.h"
#include <intrin.h>

// #include "Polyhook\PolyHook\PolyHook.hpp"
// Update 

c_hooking g_hooking;
gui g_gui;


void __stdcall hk_paint_traverse(unsigned int v, bool f, bool a)
{
#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_PAINT->getProtectionObject();
#endif

	if (g_weebwarecfg.auto_queue) {
		if (!g_weebware.g_engine->is_connected() && !g_weebware.g_engine->is_in_game()) {
			g_ai.auto_queue();
		}
		else {
			g_ai.needs_queue = true;
		}
	}
	
	if (!g_weebwarecfg.obs_proof) {
		hook_functions::paint_traverse(v, f, a);
	}
	else {
		if (strstr(g_weebware.g_panel->getname(v), "FocusOverlayPanel"))
			g_weebware.g_panel->set_mouseinput_enabled(v, g_weebware.menu_opened);

#if DEBUG_HOOKS
		 PLH::FnCast(g_hooking.paint_tramp, g_hooking.o_painttraverse)(g_weebware.g_panel, v, f, a);
#else
		g_hooking.o_painttraverse(g_weebware.g_panel, v, f, a);
#endif
	}
}


bool __stdcall hk_clientmode_cm(float input_sample_time, c_usercmd* cmd)
{
#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_CM->getProtectionObject();
	//auto orig = g_hooking.o_createmove(g_weebware.g_client_mode, input_sample_time, cmd);
#else
	auto orig = PLH::FnCast(g_hooking.cm_tramp, g_hooking.o_createmove)(g_weebware.g_client_mode, input_sample_time, cmd);
#endif


	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game()) {
		auto retv = hook_functions::clientmode_cm(input_sample_time, cmd, g_weebware.send_packet);
		if (cmd && cmd->command_number)
		{
			uintptr_t* fp;
			__asm mov fp, ebp;
			*(bool*)(*fp - 0x1C) = g_weebware.send_packet;
		}
		return retv;
	}
	else
		return g_hooking.o_createmove(g_weebware.g_client_mode, input_sample_time, cmd);

}

long __stdcall hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param)
{
#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_RESET->getProtectionObject();
#endif

	return hook_functions::reset(device, presentation_param);
}


bool font_setup = false;
long __stdcall hk_endscene(IDirect3DDevice9* device)
{

#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_ENDSCENE->getProtectionObject();
//	auto orig = g_hooking.o_endscene(device);

	if (!g_weebwarecfg.obs_proof) {
		return g_hooking.o_endscene(device);
	}
#else
	auto orig = PLH::FnCast(g_hooking.endscene_tramp, g_hooking.o_endscene)(device);
#endif


	static uintptr_t gameoverlay_return_address = 0;

	if (!gameoverlay_return_address) {
		MEMORY_BASIC_INFORMATION info;
		VirtualQuery(_ReturnAddress(), &info, sizeof(MEMORY_BASIC_INFORMATION));

		char mod[MAX_PATH];
		GetModuleFileNameA((HMODULE)info.AllocationBase, mod, MAX_PATH);

		if (strstr(mod, "gameoverlay"))
			gameoverlay_return_address = (uintptr_t)(_ReturnAddress());
	}

	if (gameoverlay_return_address != (uintptr_t)(_ReturnAddress())) {
		return g_hooking.o_endscene(device);
	}


	// uwuwuwuw streamproof ? and where all our stuff is setup and drawn
	return hook_functions::end_scene(device);
}


long __stdcall hk_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region)
{
#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_PRESENT->getProtectionObject();
//	auto orig = g_hooking.o_present(device, src, dest, wnd_override, dirty_region);
#else
	auto orig = PLH::FnCast(g_hooking.present_tramp, g_hooking.o_present)(device, src, dest, wnd_override, dirty_region);
#endif

	if (!g_weebwarecfg.obs_proof) {
		return hook_functions::present(device, src, dest, wnd_override, dirty_region);		
	}

	return g_hooking.o_present(device, src, dest, wnd_override, dirty_region);
}


void __fastcall hk_draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_DME->getProtectionObject();
#endif

	//if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game()) {
	////	hook_functions::draw_model_execute(thisptr, edx, ctx, state, pInfo, pCustomBoneToWorld);
	//	g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	//}
	//else
	//	g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

	// PLH::FnCast(g_hooking.dme_tramp, g_hooking.o_dme)(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	//
	// 
}

void __fastcall hk_scene_end(void* thisptr, void* edx) {

#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_SCENEEND->getProtectionObject();
	g_hooking.o_sceneend(thisptr, edx);
#else
	PLH::FnCast(g_hooking.sceneend_tramp, g_hooking.o_sceneend)(thisptr, edx);
#endif

	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game()) {
		hook_functions::scene_end(thisptr, edx);
	}

}

bool PrecacheModel(const char* szModelName)
{
	INetworkStringTable* m_pModelPrecacheTable = g_weebware.g_NetworkContainer->FindTable("modelprecache");

	if (m_pModelPrecacheTable)
	{
		g_weebware.g_model_info->findorloadmodel(szModelName);
		int idx = m_pModelPrecacheTable->AddString(false, szModelName);
		if (idx == (unsigned short)-1)
			return false;
	}
	return true;
}


MDLHandle_t  __fastcall hk_findmdl(void* ecx, void* edx, char* FilePath)
{

#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_MDL->getProtectionObject();
//	auto orig = g_hooking.o_mdl(ecx, edx, FilePath);
#else
	auto orig = PLH::FnCast(g_hooking.mdl_tramp, g_hooking.o_mdl)(ecx, edx, FilePath);
#endif

	if (!g_weebware.models_installed)
		return g_hooking.o_mdl(ecx, edx, FilePath);

	PrecacheModel("models/player/custom_player/caleon1/reinakousaka/reina_red.mdl");
	PrecacheModel("models/player/custom_player/caleon1/reinakousaka/reina_blue.mdl");
	PrecacheModel("models/player/custom_player/voikanaa/mirainikki/gasaiyono.mdl");
	PrecacheModel("models/player/custom_player/bbs_93x_net_2016/kimono_luka/update_2016_08_05/kimono_luka.mdl");
	PrecacheModel("models/player/custom_player/monsterko/inori_yuzuriha/inori.mdl");
	return g_hooking.o_mdl(ecx, edx, FilePath);
}

float __stdcall hk_viewmodel() {
#if DEBUG_HOOKS
	auto orig = PLH::FnCast(g_hooking.vm_tramp, g_hooking.o_vm)();
#else
	auto protecc = g_hooking.VEH_VM->getProtectionObject();
	//auto orig = g_hooking.o_vm();
#endif

	if (!g_weebwarecfg.viewmodel_changer)
		return g_hooking.o_vm();

	auto local_player = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());


	if (local_player && local_player->is_valid_player()) {
		return g_hooking.o_vm() + g_weebwarecfg.viewmodel_offset;
	}
	else {
		return g_hooking.o_vm();
	}
}

void __stdcall hk_frame_stage_notify(clientframestage_t curStage)
{
#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_FSN->getProtectionObject();
#endif

	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		hook_functions::frame_stage_notify(curStage);
	else {
#if DEBUG_HOOKS
		PLH::FnCast(g_hooking.fsn_tramp, g_hooking.o_fsn)(curStage);
#else
		g_hooking.o_fsn(curStage);
#endif
	}
}
// 80 B9 ? ? ? ? ? 74 14 F3 0F 10 81 ? ? ? ? 

void SetLocalPlayerReady()
{
	static auto SetLocalPlayerReadyFn = reinterpret_cast<bool(__stdcall*)(const char*)>(g_weebware.pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));
	if (SetLocalPlayerReadyFn)
		SetLocalPlayerReadyFn("");
}

void __fastcall hkEmitSound(void* ecx, void* edx, void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, Vector* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int& params)
{
#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_SOUNDS->getProtectionObject();
#endif

	if (!strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep") && g_weebwarecfg.misc_autoAccept)
		SetLocalPlayerReady();


#if DEBUG_HOOKS
	return PLH::FnCast(g_hooking.sound_tramp, g_hooking.o_sounds)(ecx, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, params);
#else
	return g_hooking.o_sounds(ecx, edx, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, flAttenuation, nSeed, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, params);
#endif
}

void c_hooking::hook_all_functions()
{
#define detours 0
#if detours
	PLH::CapstoneDisassembler dis(PLH::Mode::x86);
	auto paint_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_panel))[41];
	DETOUR_PAINT = new PLH::x86Detour((char*)paint_addr, (char*)&hk_paint_traverse, &paint_tramp, dis);
	DETOUR_PAINT->hook();
	o_painttraverse = reinterpret_cast<decltype(o_painttraverse)>(paint_addr);

	auto cm_addr = (*reinterpret_cast<uintptr_t * *>(g_weebware.g_client_mode))[24];
	DETOUR_CM = new PLH::x86Detour((char*)cm_addr, (char*)& hk_clientmode_cm, &cm_tramp, dis);
	DETOUR_CM->hook();
	o_createmove = reinterpret_cast<decltype(o_createmove)>(cm_addr);


	auto reset_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[16];
	DETOUR_RESET = new PLH::x86Detour((char*)reset_addr, (char*)&hk_reset, &reset_tramp, dis);
	DETOUR_RESET->hook();
	o_reset = reinterpret_cast<decltype(o_reset)>(reset_addr);

	auto present_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[17];
	DETOUR_PRESENT = new PLH::x86Detour((char*)present_addr, (char*)&hk_present, &present_tramp, dis);
	DETOUR_PRESENT->hook();
	o_present = reinterpret_cast<decltype(o_present)>(present_addr);

	// present

	auto dme_addr = (*reinterpret_cast<uintptr_t * *>(g_weebware.g_model_render))[21];
	DETOUR_DME = new PLH::x86Detour((char*)dme_addr, (char*)& hk_draw_model_execute, &dme_tramp, dis);
	DETOUR_DME->hook();
	o_dme = reinterpret_cast<decltype(o_dme)>(dme_addr);

	// scene end

	auto fsn_addr = (*reinterpret_cast<uintptr_t * *>(g_weebware.g_client))[37];
	DETOUR_FSN = new PLH::x86Detour((char*)fsn_addr, (char*)& hk_frame_stage_notify, &fsn_tramp, dis);
	DETOUR_FSN->hook();
	o_fsn = reinterpret_cast<decltype(o_fsn)>(fsn_addr);

	// mdl

	// viewmodel

	// sound

	PLH::CapstoneDisassembler dis(PLH::Mode::x86);

	auto end_scene_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[42];
	DETOUR_ENDSCENE = new PLH::x86Detour((char*)end_scene_addr, (char*)&hk_endscene, &endscene_tramp, dis);
	DETOUR_ENDSCENE->hook();
	o_endscene = reinterpret_cast<decltype(o_endscene)>(end_scene_addr);


	auto scene_end_addr = (*reinterpret_cast<uintptr_t * *>(g_weebware.g_render_view))[9];
	DETOUR_SCENEEND = new PLH::x86Detour((char*)scene_end_addr, (char*)& hk_scene_end, &sceneend_tramp, dis);
	DETOUR_SCENEEND->hook();
	o_sceneend = reinterpret_cast<decltype(o_sceneend)>(scene_end_addr);

	PLH::CapstoneDisassembler dis(PLH::Mode::x86);

	auto end_scene_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[42];
	DETOUR_ENDSCENE = new PLH::x86Detour((char*)end_scene_addr, (char*)&hk_endscene, &endscene_tramp, dis);
	DETOUR_ENDSCENE->hook();
	o_endscene = reinterpret_cast<decltype(o_endscene)>(end_scene_addr);

#else

	
#if DEBUG_HOOKS
		PLH::CapstoneDisassembler dis(PLH::Mode::x86);

		auto end_scene_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[42];
		DETOUR_ENDSCENE = new PLH::x86Detour((char*)end_scene_addr, (char*)&hk_endscene, &endscene_tramp, dis);
		DETOUR_ENDSCENE->hook();
		o_endscene = reinterpret_cast<decltype(o_endscene)>(end_scene_addr);

		auto paint_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_panel))[41];
		DETOUR_PAINT = new PLH::x86Detour((char*)paint_addr, (char*)&hk_paint_traverse, &paint_tramp, dis);
		DETOUR_PAINT->hook();
		o_painttraverse = reinterpret_cast<decltype(o_painttraverse)>(paint_addr);

		auto cm_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_client_mode))[24];
		DETOUR_CM = new PLH::x86Detour((char*)cm_addr, (char*)&hk_clientmode_cm, &cm_tramp, dis);
		DETOUR_CM->hook();
		o_createmove = reinterpret_cast<decltype(o_createmove)>(cm_addr);

		//auto present_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[17];
		//DETOUR_PRESENT = new PLH::x86Detour((char*)present_addr, (char*)&hk_present, &present_tramp, dis);
		//DETOUR_PRESENT->hook();
		//o_present = reinterpret_cast<decltype(o_present)>(present_addr);

		auto scene_end_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_render_view))[9];
		DETOUR_SCENEEND = new PLH::x86Detour((char*)scene_end_addr, (char*)&hk_scene_end, &sceneend_tramp, dis);
		DETOUR_SCENEEND->hook();
		o_sceneend = reinterpret_cast<decltype(o_sceneend)>(scene_end_addr);

		auto fsn_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_client))[37];
		DETOUR_FSN = new PLH::x86Detour((char*)fsn_addr, (char*)&hk_frame_stage_notify, &fsn_tramp, dis);
		DETOUR_FSN->hook();
		o_fsn = reinterpret_cast<decltype(o_fsn)>(fsn_addr);

		auto mdl_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_mdlcache))[10];
		DETOUR_MDL = new PLH::x86Detour((char*)mdl_addr, (char*)&hk_findmdl, &mdl_tramp, dis);
		DETOUR_MDL->hook();
		o_mdl = reinterpret_cast<decltype(o_mdl)>(mdl_addr);

		auto vm_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_client_mode))[35];
		DETOUR_VM = new PLH::x86Detour((char*)vm_addr, (char*)&hk_viewmodel, &vm_tramp, dis);
		DETOUR_VM->hook();
		o_vm = reinterpret_cast<decltype(o_vm)>(vm_addr);

		auto sound_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_enginesound))[5];
		DETOUR_SOUNDS = new PLH::x86Detour((char*)sound_addr, (char*)&hkEmitSound, &sound_tramp, dis);
		DETOUR_SOUNDS->hook();
		o_sounds = reinterpret_cast<decltype(o_sounds)>(sound_addr);
#else

	auto paint_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_panel))[41];
	VEH_PAINT = new PLH::BreakPointHook((char*)paint_addr, (char*)&hk_paint_traverse);
	VEH_PAINT->hook();
	o_painttraverse = reinterpret_cast<decltype(o_painttraverse)>(paint_addr);

	auto cm_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_client_mode))[24];
	VEH_CM = new PLH::BreakPointHook((char*)cm_addr, (char*)&hk_clientmode_cm);
	VEH_CM->hook();
	o_createmove = reinterpret_cast<decltype(o_createmove)>(cm_addr);

	auto reset_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[16];
	VEH_RESET = new PLH::BreakPointHook((char*)reset_addr, (char*)&hk_reset);
	VEH_RESET->hook();
	o_reset = reinterpret_cast<decltype(o_reset)>(reset_addr);

	auto present_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[17];
	VEH_PRESENT = new PLH::BreakPointHook((char*)present_addr, (char*)&hk_present);
	VEH_PRESENT->hook();
	o_present = reinterpret_cast<decltype(o_present)>(present_addr);

	auto scene_end_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_render_view))[9];
	VEH_SCENEEND = new PLH::BreakPointHook((char*)scene_end_addr, (char*)&hk_scene_end);
	VEH_SCENEEND->hook();
	o_sceneend = reinterpret_cast<decltype(o_sceneend)>(scene_end_addr);

	auto fsn_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_client))[37];
	VEH_FSN = new PLH::BreakPointHook((char*)fsn_addr, (char*)&hk_frame_stage_notify);
	VEH_FSN->hook();
	o_fsn = reinterpret_cast<decltype(o_fsn)>(fsn_addr);

	auto mdl_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_mdlcache))[10];
	VEH_MDL = new PLH::BreakPointHook((char*)mdl_addr, (char*)&hk_findmdl);
	VEH_MDL->hook();
	o_mdl = reinterpret_cast<decltype(o_mdl)>(mdl_addr);

	auto vm_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_client_mode))[35];
	VEH_VM = new PLH::BreakPointHook((char*)vm_addr, (char*)&hk_viewmodel);
	VEH_VM->hook();
	o_vm = reinterpret_cast<decltype(o_vm)>(vm_addr);

	auto sound_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_enginesound))[5];
	VEH_SOUNDS = new PLH::BreakPointHook((char*)sound_addr, (char*)&hkEmitSound);
	VEH_SOUNDS->hook();
	o_sounds = reinterpret_cast<decltype(o_sounds)>(sound_addr);

	auto end_scene_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_direct_x))[42];
	VEH_ENDSCENE = new PLH::BreakPointHook((char*)end_scene_addr, (char*)&hk_endscene);
	VEH_ENDSCENE->hook();
	o_endscene = reinterpret_cast<decltype(o_endscene)>(end_scene_addr);


#endif





	// DO NOT UNCOMMENT

	//auto dme_addr = (*reinterpret_cast<uintptr_t**>(g_weebware.g_model_render))[21];
	//VEH_DME = new PLH::BreakPointHook((char*)dme_addr, (char*)&hk_draw_model_execute);
	//VEH_DME->hook();
	//o_dme = reinterpret_cast<decltype(o_dme)>(dme_addr);


#endif

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

	g_weebware.old_window_proc = (WNDPROC)SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)hook_functions::hk_window_proc);

	// knife_changer::apply_proxyhooks();
}

void c_hooking::unhook_all_functions()
{
	g_weebware.menu_opened = false;
	g_weebware.g_input_system->EnableInput(true);

	// wait for shit to register
	Sleep(100);
#if DEBUG_HOOKS
	DETOUR_PAINT->unHook();
	DETOUR_CM->unHook();
	DETOUR_RESET->unHook();
	DETOUR_ENDSCENE->unHook();
	DETOUR_DME->unHook();
	DETOUR_FSN->unHook();
	DETOUR_CURSORLOCK->unHook();
#else
	VEH_PAINT->unHook();
	VEH_CM->unHook();
	VEH_RESET->unHook();
	VEH_FSN->unHook();
	VEH_PRESENT->unHook();
	VEH_SCENEEND->unHook();
	VEH_SOUNDS->unHook();
	VEH_MDL->unHook();
	VEH_VM->unHook();
	VEH_ENDSCENE->unHook();
#endif
	networking::curl_cleanup();
	SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)g_weebware.old_window_proc);
//	knife_changer::remove_proxyhooks();
	g_vars.g_unload.set(1.0f);
}