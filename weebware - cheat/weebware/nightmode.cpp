#include "Header.h"
#include "nightmode.h"


static c_convar* old_sky_name;
bool done = false;

void c_nightmode::run() {

	g_weebwarecfg.visuals_nightmode ? g_nightmode.apply() : g_nightmode.remove();

	if (!done)
		return;

	static auto r_drawspecificstaticprop = g_weebware.g_convars->find_cvar("r_DrawSpecificStaticProp");
	r_drawspecificstaticprop->SetValue(g_weebwarecfg.visuals_nightmode ? 0 : 1);

	static auto r_3dsky = g_weebware.g_convars->find_cvar("r_3dsky");
	r_3dsky->SetValue(g_weebwarecfg.visuals_nightmode ? 0 : 1);
}


void c_nightmode::apply() {
	if (done)
		return;

	c_base_entity* local_player = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!local_player)
		return;

	old_sky_name = g_weebware.g_convars->find_cvar("sv_skyname");
	float brightness = 20 / 100.f;

	for (auto i = g_weebware.g_mat_sys->first_material(); i != g_weebware.g_mat_sys->invalid_material(); i = g_weebware.g_mat_sys->next_material(i)) {
		auto* pMaterial = g_weebware.g_mat_sys->get_mat(i);

		if (!pMaterial)
			continue;

		if (strstr(pMaterial->gettexturegroupname(), "World")) {
			pMaterial->colormodulate(brightness, brightness, brightness);
		}
		else if (strstr(pMaterial->gettexturegroupname(), "StaticProp")) {
			pMaterial->colormodulate(brightness + 0.25f, brightness + 0.25f, brightness + 0.25f);
		}
	}

	g_weebwarecfg.night_sky = true;

	done = true;

}


void c_nightmode::remove() {

	if (!done)
		return;

	c_base_entity* local_player = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!local_player)
		return;

	for (auto i = g_weebware.g_mat_sys->first_material(); i != g_weebware.g_mat_sys->invalid_material(); i = g_weebware.g_mat_sys->next_material(i)) {
		auto* pMaterial = g_weebware.g_mat_sys->get_mat(i);

		if (!pMaterial)
			continue;

		if (strstr(pMaterial->gettexturegroupname(), "World")) {
			pMaterial->colormodulate(1, 1, 1);
		}
		else if (strstr(pMaterial->gettexturegroupname(), "StaticProp")) {
			pMaterial->colormodulate(1, 1, 1);
		}
		//if (strstr(pMaterial->gettexturegroupname(), ("SkyBox"))) {
		//	pMaterial->colormodulate(1, 1, 1);
		//}
	}

	if (old_sky_name)
		g_weebwarecfg.night_sky = false;

	done = false;

}