// hook_helper.h
// wrapper for minhook

#pragma once

namespace HHELPER
{
	void Start();
	void End();
	void HookFn(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
	void HookPt(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
	void HookCm(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
	void HookEs(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
	void HookRe(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
	void HookVm(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
	void HookSe(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
	void HookDme(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
}
#pragma once
