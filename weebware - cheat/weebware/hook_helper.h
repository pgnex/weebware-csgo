// hook_helper.h
// wrapper for minhook

#pragma once

namespace HHELPER
{
	void Start();
	void End();
	void HookFn(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
	void HookPt(LPVOID target, LPVOID detour_addr, LPVOID* original_addr);
}
#pragma once
