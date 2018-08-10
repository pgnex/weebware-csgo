#pragma once

#ifndef HOOKFUNCS
#define HOOKFUNCS
namespace hook_functions
{
	bool clientmode_cm(float input_sample_time, c_usercmd* cmd);
	void paint_traverse(unsigned int v, bool f, bool a);
	LRESULT __stdcall hk_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	long reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_param);
	void __stdcall hk_frame_stage_notify(clientframestage_t curStage);
	long __stdcall hk_present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND wnd_override, const RGNDATA* dirty_region);
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
	typedef void(__thiscall *pt_t)(void*, unsigned int, bool, bool); // type defs so its shorter to call original.
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
#pragma endregion

#pragma region FSN
	using fn_fsn = void(__thiscall*)(i_base_client*, clientframestage_t);
	fn_fsn o_fsn;
#pragma endregion


public:
	// Global function to initate the hooks
	void hook_all_functions();
	void unhook_all_functions();
	long(__stdcall* original_present)(IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);


};

extern c_hooking g_hooking;
#endif

