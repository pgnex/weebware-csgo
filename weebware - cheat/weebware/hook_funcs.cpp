#include "Header.h"
#include "shared.h"
#include "gui.h"
#include "key_values.h"
#include "net_channel.h"
#include <intrin.h>

// #include "Polyhook\PolyHook\PolyHook.hpp"
// Update 

c_hooking g_hooking;
gui g_gui;

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

namespace hooks {

	vfunc_hook vfunc_paint;
	vfunc_hook vfunc_cm;
	vfunc_hook vfunc_es;
	vfunc_hook vfunc_rs;
	vfunc_hook vfunc_emit;
	vfunc_hook vfunc_se;
	vfunc_hook vfunc_fsn;
	vfunc_hook vfunc_vm;
	vfunc_hook vfunc_dme;
	vfunc_hook vfunc_ov;
	vfunc_hook vfunc_svc;
	vfunc_hook vfunc_snm;

	namespace hook_index {
		int pt = 41;
		int cm = 24;
		int es = 42;
		int re = 16;
		int em = 5;
		int se = 9;
		int fsn = 37;
		int vm = 35;
		int dme = 21;
		int ov = 18;
		int svc = 13;
		int snm = 40;
	}

	uint32_t GetVirtual(void* class_ptr, uint32_t index) {
		return (*(uint32_t**)(class_ptr))[index];
	}

	void init_hooks() {
		vfunc_paint.setup(g_weebware.g_panel, "vgui2.dll");
		vfunc_paint.hook_index(hook_index::pt, hk_paint_traverse);

		vfunc_cm.setup(g_weebware.g_client_mode, "client.dll");
		vfunc_cm.hook_index(hook_index::cm, hk_clientmode_cm);

		vfunc_es.setup(g_weebware.g_direct_x, "shaderapidx9.dll");
		vfunc_es.hook_index(hook_index::es, hk_endscene);

		vfunc_rs.setup(g_weebware.g_direct_x, "shaderapidx9.dll");
		vfunc_rs.hook_index(hook_index::re, hk_reset);

		vfunc_emit.setup(g_weebware.g_enginesound, "engine.dll");
		vfunc_emit.hook_index(hook_index::em, hk_emitsound);

		vfunc_se.setup(g_weebware.g_render_view, "engine.dll");
		vfunc_se.hook_index(hook_index::se, hk_scene_end);

		vfunc_fsn.setup(g_weebware.g_client, "client.dll");
		vfunc_fsn.hook_index(hook_index::fsn, hk_frame_stage_notify);

		vfunc_vm.setup(g_weebware.g_client_mode, "client.dll");
		vfunc_vm.hook_index(hook_index::vm, hk_viewmodel);

		vfunc_dme.setup(g_weebware.g_model_render, "engine.dll");
		vfunc_dme.hook_index(hook_index::dme, hk_draw_model_execute);

		vfunc_ov.setup(g_weebware.g_client_mode, "client.dll");
		vfunc_ov.hook_index(hook_index::ov, hk_overrideview);

		c_convar* sv_cheats_con = g_weebware.g_convars->find_cvar("sv_cheats");
		vfunc_svc.setup(sv_cheats_con, "client.dll");
		vfunc_svc.hook_index(hook_index::svc, hk_svcheats);
	}

	void unhook() {

		// unhooking 
		// DETOUR_SOUNDS->unHook();
		// VEH_PAINT->unHook();

		vfunc_paint.unhook_all();
		vfunc_cm.unhook_all();
		vfunc_es.unhook_all();
		vfunc_rs.unhook_all();
		vfunc_emit.unhook_all();
		vfunc_se.unhook_all();
		vfunc_fsn.unhook_all();
		vfunc_vm.unhook_all();
	}


	// paint traverse
	painttraverse o_pt = nullptr;
	void __stdcall hk_paint_traverse(unsigned int v, bool f, bool a) {
		// o_pt = vfunc_paint.get_original<painttraverse>(hook_index::pt);
		o_pt(g_weebware.g_panel, v, f, a);

		if (g_weebwarecfg.auto_queue) {
			if (!g_weebware.g_engine->is_connected() && !g_weebware.g_engine->is_in_game()) {
				g_ai.auto_queue();
			}
			else {
				g_ai.needs_queue = true;
			}
		}

		hooks::hook_functions::paint_traverse(v, f, a);

	}

	bool __fastcall	hkSendNetMsg( net_channel* thisptr, int edx, INetMessage* pMessage, bool bForceReliable, bool bVoice )
	{
		static auto oSendNetMsg = vfunc_snm.get_original<sendnetmsg>( hook_index::snm );

		/*
		 * @note: disable files crc check (sv_pure)
		 * dont send message if it has FileCRCCheck type
		 */


		 //&& Widgets::bypassSvPure->GetState()
		if ( pMessage->GetType( ) == 14 )
			return false;

		/*
		 * @note: fix lag with chocking packets when voice chat is active
		 * check for voicedata group and enable voice stream
		 * @credits: Flaww
		 */
		if ( pMessage->GetGroup( ) == 9 )
			bVoice = true;

		return oSendNetMsg( edx, pMessage, bForceReliable, bVoice );
	}


	// createmove
	createmove o_cm = nullptr;
	bool __fastcall hk_clientmode_cm(void* e, void* z, float input_sample_time, c_usercmd* cmd) {
		if (cmd && cmd->command_number) {
			uintptr_t* frame_ptr;
			__asm mov frame_ptr, ebp;
			bool& send_packet = *reinterpret_cast<bool*>(*frame_ptr - 0x34);
			hooks::hook_functions::clientmode_cm(input_sample_time, cmd, send_packet);

			// CLAMPS PLEASE NO BAN ?
			g_maths.clamp_angle(cmd->viewangles);
			cmd->forwardmove = std::clamp(cmd->forwardmove, -450.f, 450.f);
			cmd->sidemove = std::clamp(cmd->sidemove, -450.f, 450.f);
			cmd->upmove = std::clamp(cmd->upmove, -450.f, 450.f);

		}
		return false; // went from returning original to false if not valid cmd or cmd_number
	}


	// endscene
	bool font_setup = false;
	endscene o_es = nullptr;
	long __stdcall hk_endscene(IDirect3DDevice9* device) {

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
			return o_es(device);
		}

		// uwuwuwuw streamproof ? and where all our stuff is setup and drawn
		hooks::hook_functions::end_scene(device);
		return o_es(device);

	}

	// reset
	reset o_rs = nullptr;
	long __stdcall hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param) {
		hooks::hook_functions::reset(device, presentation_param, (o_rs(device, presentation_param) >= 0));
		std::cout << "hooked reset" << std::endl;
		return o_rs(device, presentation_param);
	}

	// emitsound
	void __fastcall hk_emitsound(void* ecx, void* edx, void* filter, int entity_index, int channel, const char* sound_entry, uint32_t sound_entry_hash, const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const Vector* origin, const Vector* direction, void* utl_origins, bool update_positions, float sound_time, int speaker_entity, void* parameters) {
		auto o_em = vfunc_emit.get_original<emitsound>(hook_index::em);

		if (!strcmp(sound_entry, "UIPanorama.popup_accept_match_beep") && g_weebwarecfg.misc_autoAccept) {
			static auto set_ready = reinterpret_cast<bool(__stdcall*)(const char*)>(g_weebware.pattern_scan("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));
			if (set_ready)
				set_ready("");
		}

		return o_em(ecx, edx, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin, direction, utl_origins, update_positions, sound_time, speaker_entity, parameters);
	}

	// sceneend
	sceneend o_se = nullptr;
	void __fastcall hk_scene_end(void* thisptr, void* edx) {
		o_se(thisptr, edx);
		if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
			hooks::hook_functions::scene_end(thisptr, edx);
	}

	// dme
	drawmodelexecute o_dme = nullptr;
	void __fastcall hk_draw_model_execute( void* thisptr, void*, void* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld ) {
		//if ( !g_weebwarecfg.visuals_chams > 0 ) {
		//	o_dme( g_weebware.g_model_render, ctx, state, pInfo, pCustomBoneToWorld );
		//	return;
		//}

		//if ( g_weebware.g_engine->is_connected( ) && g_weebware.g_engine->is_in_game( ) ) {
		//	if ( pInfo.pModel ) {
		//		// MOVE THIS CHECK IF I EVER ADD ANYTHING ELSE TO DME HOOK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//		if ( g_weebwarecfg.visuals_chams_xqz )
		//			if ( !g_weebwarecfg.visuals_glow_enabled )
		//				hooks::hook_functions::draw_model_execute( thisptr, ctx, state, pInfo, pCustomBoneToWorld, o_dme );
		//	}
		//}
		o_dme( g_weebware.g_model_render, ctx, state, pInfo, pCustomBoneToWorld );
	}

	// framestagenotify
	framestagenotify o_fsn = nullptr;
	void __stdcall hk_frame_stage_notify(clientframestage_t curStage) {
		if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
			hooks::hook_functions::frame_stage_notify(curStage);

		o_fsn(g_weebware.g_client, curStage);
	}

	// hook svcheats for thirdperson fix..
	bool __fastcall hk_svcheats(PVOID pConVar, void* edx) {
		static auto ofunc = vfunc_svc.get_original<bool(__thiscall*)(PVOID)>(hook_index::svc);
		if (!ofunc)
			return false;

		if (!g_weebwarecfg.thirdperson)
			return ofunc(pConVar);

		static auto dwCAM_Think = g_weebware.pattern_scan("client.dll", "85 C0 75 30 38 86");
		if (reinterpret_cast<uintptr_t>(_ReturnAddress()) == dwCAM_Think)
			return true;

		return ofunc(pConVar);
	}

	// override view
	overrideview o_ov = nullptr;
	void __fastcall hk_overrideview(void* thisptr, int edx, view_setup_t* vsView) {
		if (g_weebwarecfg.thirdperson && vsView)
			overrideview::thirdperson();
		else if (g_weebware.g_input->bCameraInThirdPerson && !g_weebwarecfg.thirdperson) {
			g_weebware.g_input->bCameraInThirdPerson = false;
			g_weebware.g_input->vecCameraOffset.z = 0;
		}
		o_ov(thisptr, edx, vsView);
	}


	// viewmodel
	viewmodel o_vm = nullptr;
	float __stdcall hk_viewmodel() {
		if (!g_weebwarecfg.viewmodel_changer)
			return o_vm();
		
		c_base_entity* local_player = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
		if (local_player && local_player->is_valid_player()) {
			return o_vm() + g_weebwarecfg.viewmodel_offset;
		}

		return o_vm();
	}


	void Setup() {
		HHELPER::Start();

		// pt hook
		HHELPER::HookPt((void*)(GetVirtual(g_weebware.g_panel, hook_index::pt)),
			(void*)(&hk_paint_traverse),
			(void**)(&o_pt));

		// createmove hook ( CAUSING BANS NEED TP DEBUG!!!)
		HHELPER::HookCm((void*)(GetVirtual(g_weebware.g_client_mode, hook_index::cm)),
			(void*)(&hk_clientmode_cm),
			(void**)(&o_cm));

		// endscene hook
		HHELPER::HookEs((void*)(GetVirtual(g_weebware.g_direct_x, hook_index::es)),
			(void*)(&hk_endscene),
			(void**)(&o_es));

		// reset hook
		HHELPER::HookRe((void*)(GetVirtual(g_weebware.g_direct_x, hook_index::re)),
			(void*)(&hk_reset),
			(void**)(&o_rs));

		// viewmodel hook
		HHELPER::HookVm((void*)(GetVirtual(g_weebware.g_client_mode, hook_index::vm)),
			(void*)(&hk_viewmodel),
			(void**)(&o_vm));

		// sceneend hook
		HHELPER::HookSe((void*)(GetVirtual(g_weebware.g_render_view, hook_index::se)),
			(void*)(&hk_scene_end),
			(void**)(&o_se));

		// drawmodel execute hook
		HHELPER::HookDme((void*)(GetVirtual(g_weebware.g_model_render, hook_index::dme)),
			(void*)(&hk_draw_model_execute),
			(void**)(&o_dme));

		// fsn hook
		HHELPER::HookFsn((void*)(GetVirtual(g_weebware.g_client, hook_index::fsn)),
			(void*)(&hk_frame_stage_notify),
			(void**)(&o_fsn));

		// overrideview hook
		HHELPER::HookOv((void*)(GetVirtual(g_weebware.g_client_mode, hook_index::ov)),
			(void*)(&hk_overrideview),
			(void**)(&o_ov));

		// emitsound hook

		//c_convar* sv_cheats_con = g_weebware.g_convars->find_cvar("sv_cheats");
		//vfunc_svc.setup(sv_cheats_con, "client.dll");
		//vfunc_svc.hook_index(hook_index::svc, hk_svcheats);

		HHELPER::End();
	}

}


bool __fastcall hkCheckFileCRCsWithServer(void* ecx, void* edx) {
	std::cout << "bytpass" << std::endl;
	return true;
}



long __stdcall hk_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region)
{
#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_PRESENT->getProtectionObject();
#endif

//	if (!g_weebwarecfg.obs_proof) {
	return hooks::hook_functions::present(device, src, dest, wnd_override, dirty_region);		
//	}

#if DEBUG_HOOKS 
    return PLH::FnCast(g_hooking.present_tramp, g_hooking.o_present)(device, src, dest, wnd_override, dirty_region);
#else
	return g_hooking.o_present(device, src, dest, wnd_override, dirty_region);
#endif

}


void __fastcall hk_draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld)
{
#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_DME->getProtectionObject();
#endif

	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game()) {


	//	hooks::hook_functions::draw_model_execute(thisptr, edx, ctx, state, pInfo, pCustomBoneToWorld);
		g_hooking.o_dme(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	}

	//	 PLH::FnCast(g_hooking.dme_tramp, g_hooking.o_dme)(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
}




MDLHandle_t  __fastcall hk_findmdl(void* ecx, void* edx, char* FilePath)
{

#if !DEBUG_HOOKS
	auto protecc = g_hooking.VEH_MDL->getProtectionObject();
#endif

	if (!g_weebware.models_installed) {
#if DEBUG_HOOKS 
		return PLH::FnCast(g_hooking.mdl_tramp, g_hooking.o_mdl)(ecx, edx, FilePath);
#else
		return g_hooking.o_mdl(ecx, edx, FilePath);
#endif
	}
	//PrecacheModel("models/player/custom_player/caleon1/reinakousaka/reina_red.mdl");
	//PrecacheModel("models/player/custom_player/caleon1/reinakousaka/reina_blue.mdl");
	//PrecacheModel("models/player/custom_player/voikanaa/mirainikki/gasaiyono.mdl");
	//PrecacheModel("models/player/custom_player/bbs_93x_net_2016/kimono_luka/update_2016_08_05/kimono_luka.mdl");
	//PrecacheModel("models/player/custom_player/monsterko/inori_yuzuriha/inori.mdl");

#if DEBUG_HOOKS 
	return PLH::FnCast(g_hooking.mdl_tramp, g_hooking.o_mdl)(ecx, edx, FilePath);
#else
	return g_hooking.o_mdl(ecx, edx, FilePath);
#endif
}



void c_hooking::hook_all_functions() {

	hooks::Setup();
	g_weebware.old_window_proc = (WNDPROC)SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)hooks::hook_functions::hk_window_proc);
	
	// knife_changer::apply_proxyhooks();
}

void c_hooking::unhook_all_functions()
{
	g_weebware.menu_opened = false;
	g_weebware.g_input_system->EnableInput(true);

	// wait for shit to register
	Sleep(100);

	hooks::unhook();

	networking::curl_cleanup();
	SetWindowLongPtr(g_weebware.h_window, GWL_WNDPROC, (LONG_PTR)g_weebware.old_window_proc);
	//	knife_changer::remove_proxyhooks();
	g_vars.g_unload.set(1.0f);
}