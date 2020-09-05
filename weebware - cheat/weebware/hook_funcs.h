#pragma once
#include "PolyHook2/PolyHook_2_0/headers/IHook.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Misc.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Exceptions/BreakPointHook.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Detour/ADetour.hpp"
#include "PolyHook2/PolyHook_2_0/headers/Detour/x86Detour.hpp"
#include "PolyHook2/PolyHook_2_0/headers/CapstoneDisassembler.hpp"
#include "shared.h"

#ifndef HOOKFUNCS
#define HOOKFUNCS
namespace hook_functions
{
	bool clientmode_cm(float input_sample_time, c_usercmd* cmd, bool& sendpacket);
	void paint_traverse(unsigned int v, bool f, bool a);
	LRESULT __stdcall hk_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	long reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param);
	void frame_stage_notify(int curStage);
	long present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region);
	long end_scene(IDirect3DDevice9* device);
	void draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld);
	void scene_end(void* thisptr, void* edx);
}

class c_hooking
{
public:
#if DEBUG_HOOKS
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
#else

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


	//PLH::x86Detour* DETOUR_ENDSCENE;
	//uint64_t endscene_tramp = NULL;
	PLH::x86Detour* DETOUR_PRESENT;
	uint64_t present_tramp = NULL;

#endif

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
#endif

