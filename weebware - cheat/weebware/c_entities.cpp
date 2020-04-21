#include "Header.h"

c_basecombat_weapon * c_base_entity::m_pActiveWeapon()
{
	static int iOffset = retrieve_offset("DT_BaseCombatCharacter", "m_hActiveWeapon");
	return (c_basecombat_weapon*)(g_weebware.g_entlist->getcliententityfromhandle((HANDLE)*(PULONG)((DWORD)this + iOffset)));
}

void c_base_entity::set_angles(Vector angles) 
{
	typedef void(__thiscall* set_angle_fn)(void*, const Vector&);
	static set_angle_fn set_angle = (set_angle_fn)g_weebware.pattern_scan("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1");
	set_angle(this, angles);
}