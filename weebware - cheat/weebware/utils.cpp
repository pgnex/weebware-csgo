#include "Header.h"

void c_utils::set_name(const char* name) {
	static c_convar* name_cvar = g_weebware.g_convars->find_cvar("name");
	*(int*)((DWORD)& name_cvar->fnChangeCallback + 0xC) = NULL;
	name_cvar->SetValue("\n\xAD\xAD\xAD");
	name_cvar->SetValue(name);
}