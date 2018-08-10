// The entry point for the program.
#include "Header.h"
#include "shared.h"

unsigned __stdcall entry_thread(void* v_arg);

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID lp_res)
{
	UNREFERENCED_PARAMETER(lp_res);

	// If load, start our thread

	if (reason == DLL_PROCESS_ATTACH)
	{
		g_weebware.h_module = mod;

		_beginthreadex(nullptr, 0, &entry_thread, nullptr, 0, nullptr);
	}
	return TRUE;
}
