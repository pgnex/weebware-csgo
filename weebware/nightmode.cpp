#include "Header.h"
#include "nightmode.h"

c_nightmode g_nightmode;

void c_nightmode::night_mode() {
	c_convar* prop_var = g_weebware.g_convars->find_cvar("r_DrawSpecificStaticProp");
	if (prop_var->fValue != 0) {
		prop_var->SetValue("1");
	}
	for (auto i = g_weebware.g_mat_sys->first_material(); i != g_weebware.g_mat_sys->invalid_material(); i = g_weebware.g_mat_sys->next_material(i)) {
		auto *pMaterial = g_weebware.g_mat_sys->get_mat(i);

		if (!pMaterial)
			continue;

		c_color col = g_weebwarecfg.nightmode_col;

		if (strstr(pMaterial->gettexturegroupname(), "World"))
		{
			pMaterial->colormodulate(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f);

			pMaterial->setmaterialvarflag(material_var_translucent, false);
		}
		if (strstr(pMaterial->gettexturegroupname(), "StaticProp"))
		{
			pMaterial->colormodulate(0.3f, 0.3f, 0.3f);
		}
		if (strstr(pMaterial->gettexturegroupname(), "Model"))
		{
			pMaterial->colormodulate(0.6f, 0.6f, 0.6f);
		}
	}
}


//// should only apply once at a time
//void c_dme::night_mode()
//{
//	if (!g_weebwarecfg.visuals_nightmode)
//		return;
//
//	// Update not required.
//	if (!night_mode_update)
//		return;
//
//	auto prop_var = g_weebware.g_convars->find_cvar("r_DrawSpecificStaticProp");
//
//	if (prop_var->fValue != 0)
//		prop_var->SetValue("1");
//
//	for (auto i = g_weebware.g_mat_sys->first_material(); i != g_weebware.g_mat_sys->invalid_material(); i = g_weebware.g_mat_sys->next_material(i)) {
//
//		auto pMaterial = g_weebware.g_mat_sys->get_mat(i);
//
//		if (!pMaterial)
//			continue;
//
//		// ghetto fix.
//		if (strstr(pMaterial->gettexturegroupname(), "World"))
//		{
//			pMaterial->colormodulate(25.f / 255.0f, 25.f / 255.0f, 25.f / 255.0f);
//
//			pMaterial->setmaterialvarflag(material_var_translucent, false);
//		}
//		if (strstr(pMaterial->gettexturegroupname(), "StaticProp"))
//		{
//			pMaterial->colormodulate(0.3f, 0.3f, 0.3f);
//		}
//		if (strstr(pMaterial->gettexturegroupname(), "Model"))
//		{
//			pMaterial->colormodulate(0.6f, 0.6f, 0.6f);
//		}
//	}
//
//	// Finished modulating world.
//	// Change sky to a dark sky.
//	// char __thiscall R_LoadNamedSkys(int this)
//	// char __fastcall R_LoadNamedSkys(const char *a1)
//	// static auto r_loadnamedsky = (char(__thiscall*)(const char* a1))g_weebware.pattern_scan("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? 33 F6 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ?");
//	// r_loadnamedsky("sky_csgo_night02b");
//	// Just loaded the materials
//	std::cout << "kite gay" << std::endl;
//	night_mode_update = false;
//}