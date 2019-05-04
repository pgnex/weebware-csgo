#include "Header.h"

c_basecombat_weapon * c_base_entity::m_pActiveWeapon()
{
	static int iOffset = retrieve_offset("DT_BaseCombatCharacter", "m_hActiveWeapon");
	return (c_basecombat_weapon*)(g_weebware.g_entlist->getcliententityfromhandle((HANDLE)*(PULONG)((DWORD)this + iOffset)));
}


