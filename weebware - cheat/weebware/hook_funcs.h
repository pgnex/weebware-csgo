#pragma once
#include "PolyHook2/PolyHook_2_0/headers/IHook.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Misc.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Exceptions/BreakPointHook.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Detour/ADetour.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Detour/x86Detour.hpp"
#include "PolyHook2/PolyHook_2_0/headers/CapstoneDisassembler.hpp"
#include "shared.h"

#define HOOKFUNCS



namespace hooks {
	void init_hooks();
	void unhook();
	void Setup();

	extern vfunc_hook vfunc_paint;
	extern vfunc_hook vfunc_cm;
	extern vfunc_hook vfunc_es;
	extern vfunc_hook vfunc_rs;
	extern vfunc_hook vfunc_emit;
	extern vfunc_hook vfunc_se;
	extern vfunc_hook vfunc_vm;
	extern vfunc_hook vfunc_dme;
	extern vfunc_hook vfunc_ov;
	extern vfunc_hook vfunc_svc;
	extern vfunc_hook vfunc_snm;

	using painttraverse = void(__thiscall*)(c_panel*, unsigned int, bool, bool);
	using createmove = bool(__fastcall*)(void*, void*, float, c_usercmd*);
	using endscene = long(__stdcall*)(IDirect3DDevice9*);
	using reset = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	using emitsound = void(__fastcall*)(void*, void*, void*, int, int, const char*, uint32_t, const char*, float, float, int, int, int, const Vector*, const Vector*, void*, bool, float, int, void*);
	using sceneend = void(__fastcall*)(void*, void*);
	using framestagenotify = void(__thiscall*)(i_base_client*, clientframestage_t);
	using viewmodel = float(__fastcall*)();
	using drawmodelexecute = void(__thiscall*)(void*, void*, const c_unknownmat_class&, const modelrenderinfo_t&, matrix3x4_t*);
	using overrideview = void(__fastcall*)(void*, int, view_setup_t*);
	using sendnetmsg = bool( __thiscall* )( int, INetMessage*, bool, bool );

	void __stdcall hk_paint_traverse(unsigned int v, bool f, bool a);
	bool __fastcall hk_clientmode_cm(void*, void*, float frameTime, c_usercmd* cmd);
	long __stdcall hk_endscene(IDirect3DDevice9* device);
	long __stdcall hk_reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param);
	void __fastcall hk_emitsound(void* ecx, void* edx, void* filter, int entity_index, int channel, const char* sound_entry, uint32_t sound_entry_hash, const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const Vector* origin, const Vector* direction, void* utl_origins, bool update_positions, float sound_time, int speaker_entity, void* parameters);
	void __fastcall hk_scene_end(void* thisptr, void* edx);
	void __stdcall hk_frame_stage_notify(clientframestage_t curStage);
	void __fastcall hk_draw_model_execute(void* thisptr, void*, void* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	float __stdcall hk_viewmodel();
	void __fastcall hk_overrideview(void* thisptr, int, view_setup_t* vsView);
	bool __fastcall hk_svcheats(PVOID pConVar, void* edx);
	bool __fastcall	hkSendNetMsg( net_channel* thisptr, int edx, INetMessage* pMessage, bool bForceReliable, bool bVoice );

	namespace hook_functions {
		bool clientmode_cm(float input_sample_time, c_usercmd* cmd, bool& sendpacket);
		void paint_traverse(unsigned int v, bool f, bool a);
		LRESULT __stdcall hk_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param, bool valid);
		void frame_stage_notify(int curStage);
		long present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region);
		long end_scene(IDirect3DDevice9* device);
		void draw_model_execute(void* thisptr, void* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld, hooks::drawmodelexecute o_dme);
		void scene_end(void* thisptr, void* edx);
		void override_view(void* thisptr, int, view_setup_t* vsView);
	}
}


class c_hooking
{
public:

	uint64_t paint_tramp = NULL;
	uint64_t lock_tramp = NULL;
	uint64_t cm_tramp = NULL;
	uint64_t reset_tramp = NULL;
	uint64_t endscene_tramp = NULL;
	uint64_t dme_tramp = NULL;
	uint64_t fsn_tramp = NULL;
	uint64_t present_tramp = NULL;
	uint64_t vm_tramp = NULL;
	uint64_t mdl_tramp = NULL;
	uint64_t sound_tramp = NULL;
	uint64_t sceneend_tramp = NULL;
	uint64_t override_tramp = NULL;

	PLH::x86Detour* DETOUR_PAINT;
	PLH::x86Detour* DETOUR_CM;
	PLH::x86Detour* DETOUR_RESET;
	PLH::x86Detour* DETOUR_FSN;
	PLH::x86Detour* DETOUR_PRESENT;
	PLH::x86Detour* DETOUR_ENDSCENE;
	PLH::x86Detour* DETOUR_DME;
	PLH::x86Detour* DETOUR_CURSORLOCK;
	PLH::x86Detour* DETOUR_SCENEEND;
	PLH::x86Detour* DETOUR_MDL;
	PLH::x86Detour* DETOUR_VM;
	PLH::x86Detour* DETOUR_SOUNDS;
	PLH::x86Detour* DETOUR_OVERRIDE;

	PLH::BreakPointHook* VEH_PAINT;
	PLH::BreakPointHook* VEH_CM;
	PLH::BreakPointHook* VEH_RESET;
	PLH::BreakPointHook* VEH_FSN;
	PLH::BreakPointHook* VEH_PRESENT;
	PLH::BreakPointHook* VEH_ENDSCENE;
	PLH::BreakPointHook* VEH_DME;
	PLH::BreakPointHook* VEH_SCENEEND;
	PLH::BreakPointHook* VEH_CURSORLOCK;
	PLH::BreakPointHook* VEH_HideGrenade;
	PLH::BreakPointHook* VEH_SOUNDS;
	PLH::BreakPointHook* VEH_MDL;
	PLH::BreakPointHook* VEH_VM;
	PLH::BreakPointHook* VEH_OVERRIDE;
	PLH::BreakPointHook* VEH_CRS_CHECK;
	PLH::BreakPointHook* VEH_FILE_SYSTEM;

private:

	LPVOID m_original_area;
	int index = 0;
	template<typename t>
	LPVOID create_original(t& type) {
		LPVOID original = &type;
		*(LPVOID*)original = (LPVOID)((DWORD)m_original_area + (index * 30));
		index++;
		return *(LPVOID*)original;
	}


#pragma region paint_traverse
public:
	typedef void(__thiscall *pt_t)(void*, unsigned int, bool, bool);
	pt_t o_painttraverse;
#pragma endregion
private:

#pragma region create_move
public:
	using fn_createmove = bool(__thiscall*)(void*, float, c_usercmd*);
	fn_createmove o_createmove;
#pragma endregion

#pragma region override_view
public:
	using fn_overrideview = void(__stdcall*)(void*, void*, view_setup_t*);
	fn_overrideview o_overrideview;
#pragma endregion

private:
#pragma region direct_x
public:
	using fn_reset = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	fn_reset o_reset;

	using fn_endscene = long(__stdcall*)(IDirect3DDevice9*);
	fn_endscene o_endscene;

	using fn_present = long(__stdcall*)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
	fn_present o_present;
#pragma endregion

#pragma region FSN
	using fn_fsn = void(__stdcall*)(int);
	fn_fsn o_fsn;

#if 0
	using fn_cursorlock = void(__thiscall*)(void*);
	fn_cursorlock o_cursor;
#else
	typedef BOOL(WINAPI *fn_setcursorpos)(int, int);
	fn_setcursorpos o_cursor;
#endif

	using grenade_fn = bool(__thiscall*)(void*);
	grenade_fn o_grenade;
#pragma endregion

#pragma region DME
	typedef void(__thiscall* fn_dme)(void*, c_unknownmat_class*, const c_unknownmat_class&, const modelrenderinfo_t&, matrix3x4*);
	fn_dme o_dme;
#pragma endregion

#pragma region SceneEnd
	typedef void(__fastcall* sceneend_fn)(void*, void*);
	sceneend_fn o_sceneend;
#pragma endregion

#pragma region Sounds
	typedef void(__fastcall* fn_sound)(void* ecx, void* edx, void* filter, int entity_index, int channel, const char* sound_entry, uint32_t sound_entry_hash, const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const Vector* origin, const Vector* direction, void* utl_origins, bool update_positions, float sound_time, int speaker_entity, void* parameters);
	fn_sound o_sounds;
#pragma endregion

#pragma region findmdl
	typedef MDLHandle_t(__fastcall* fn_mdl)(void* ecx, void* edx, char* FilePath);
	fn_mdl o_mdl;
#pragma endregion

#pragma region viewmodel
	typedef float(__fastcall* fn_vm)();
	fn_vm o_vm;
#pragma endregion

public:
	// Global function to initate the hooks
	void hook_all_functions();
	void unhook_all_functions();

#pragma region GameOverlay
	long(__stdcall* original_present)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
	long(__stdcall* original_reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
#pragma endregion

};

extern c_hooking g_hooking;

