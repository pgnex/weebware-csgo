#include "Header.h"
#include "legit_aa.h"

LegitAntiAim g_LegitAntiAim;

float updateTime2;
float lastUpdate2;
float wasmoving2;
bool performBreak2;
bool m_bBreakBalance2 = false;

float max_delta(c_base_entity* pEnt) {

	auto animstate = uintptr_t(pEnt->AnimState());

	float duckammount = *(float*)(animstate + 0xA4);
	float speedfraction = max(0, min(*reinterpret_cast<float*>(animstate + 0xF8), 1));

	float speedfactor = max(0, min(1, *reinterpret_cast<float*> (animstate + 0xFC)));

	float unk1 = ((*reinterpret_cast<float*> (animstate + 0x11C) * -0.30000001f) - 0.19999999f)* speedfraction;
	float unk2 = unk1 + 1.f;
	float unk3;

	if (duckammount > 0) {
		unk2 += ((duckammount * speedfactor) * (0.5f - unk2));
	}

	unk3 = *(float*)(animstate + 0x334) * unk2;

	return unk3;
}


void LegitAntiAim::Run(c_usercmd* cmd)
{
	auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!local)
		return;

	if (!g_weebwarecfg.misc_legit_aa_enabled)
		return;


}