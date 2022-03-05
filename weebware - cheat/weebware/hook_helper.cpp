// hook_helper.cpp
// to-do add unhooks

#include "Header.h"
#include "hook_helper.h"
#include "minhook/include/MinHook.h"
#pragma comment(lib, "minhook/build/VC15/lib/Release/libMinHook.x86.lib")

namespace HHELPER
{
	void Start()
	{
		MH_Initialize();
	}

	void End()
	{
		MH_EnableHook(nullptr);
	}

	// example call:
	// hook_fn( lock_cursor_target, reinterpret_cast<LPVOID>( &surface::lock_cursor ), reinterpret_cast<void**>( &lock_cursor_original ) );
	void HookFn(LPVOID target, LPVOID detour_addr, LPVOID* original_addr)
	{
		MH_CreateHook(target, detour_addr, original_addr);
	}

	void HookPt(LPVOID target, LPVOID detour_addr, LPVOID* original_addr)
	{
		MH_CreateHook(target, detour_addr, original_addr);
	}

	void HookCm(LPVOID target, LPVOID detour_addr, LPVOID* original_addr)
	{
		MH_CreateHook(target, detour_addr, original_addr);
	}

	void HookEs(LPVOID target, LPVOID detour_addr, LPVOID* original_addr)
	{
		MH_CreateHook(target, detour_addr, original_addr);
	}

	void HookRe(LPVOID target, LPVOID detour_addr, LPVOID* original_addr)
	{
		MH_CreateHook(target, detour_addr, original_addr);
	}

	void HookVm(LPVOID target, LPVOID detour_addr, LPVOID* original_addr)
	{
		MH_CreateHook(target, detour_addr, original_addr);
	}

	void HookSe(LPVOID target, LPVOID detour_addr, LPVOID* original_addr)
	{
		MH_CreateHook(target, detour_addr, original_addr);
	}

	void HookDme(LPVOID target, LPVOID detour_addr, LPVOID* original_addr)
	{
		MH_CreateHook(target, detour_addr, original_addr);
	}
}