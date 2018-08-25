#pragma once

#ifndef HOOKFUNCS
#define HOOKFUNCS
namespace hook_functions
{
	bool clientmode_cm(float input_sample_time, c_usercmd* cmd);
	void paint_traverse(unsigned int v, bool f, bool a);
	LRESULT __stdcall hk_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	long reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param);
	void frame_stage_notify(clientframestage_t curStage);
	long __stdcall hk_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region);
	long end_scene(IDirect3DDevice9* device);
	void draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld);
}

class c_hooking
{
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

private:
#pragma region direct_x
public:
	using fn_reset = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	fn_reset o_reset;
	using fn_endscene = long(__stdcall*)(IDirect3DDevice9*);
	fn_endscene o_endscene;
#pragma endregion

#pragma region FSN
	using fn_fsn = void(__stdcall*)(clientframestage_t);
	fn_fsn o_fsn;

	using fn_cursorlock = void(__thiscall*)(void*);
	fn_cursorlock o_cursor;
#pragma endregion

#pragma region DME
	typedef void(__thiscall* fn_dme)(void*,  c_unknownmat_class*, const c_unknownmat_class&, const modelrenderinfo_t&, matrix3x4*);
	fn_dme o_dme;
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

