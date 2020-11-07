#include "Header.h"
#include "shared.h"
#include "scene_end.h"
#include "hook_funcs.h"
#include <iostream>

c_sceneend g_sceneend;

void hooks::hook_functions::scene_end( void* thisptr, void* edx ) {

	if ( g_weebware.g_engine->is_taking_screenshot( ) && g_weebwarecfg.screenshot_proof )
		return;

	if ( g_weebwarecfg.visuals_chams > 0 ) {
		if ( !g_weebwarecfg.visuals_chams_xqz || g_weebwarecfg.visuals_glow_enabled )
			chams::se::player_chams( );
	}

	if ( g_weebwarecfg.visuals_glow_enabled )
		g_sceneend.glow( );
}


void c_sceneend::glow() {

	auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
	c_color col;

	for (auto i = 0; i < g_weebware.g_glow_obj_manager->m_GlowObjectDefinitions.Count(); i++)
	{
		auto& glowObject = g_weebware.g_glow_obj_manager->m_GlowObjectDefinitions[i];
		auto entity = reinterpret_cast<c_base_entity*>(glowObject.m_pEntity);

		// nullptr check
		if (!entity)
			continue;
		// make sure isnt being used already
		if (glowObject.IsUnused())
			continue;
		// check dormant, not valid player because i want to glow on objects
		if (entity->is_dormant())
			continue;

		// apply based on type of entity
		auto class_id = entity->get_client_class()->m_ClassID;

		if (strstr(entity->get_client_class()->m_networkedname, "CWeapon"))
			class_id = -1;
		else if (class_id == 1 || class_id == weapon_revolver || class_id == 46)
			class_id = -1;

		switch (class_id) {
		case 40:
			if (!g_weebwarecfg.visuals_glow_player) continue;
			if (g_weebwarecfg.visuals_glow_hidden_col) col = is_visible(local, entity) ? g_weebwarecfg.visuals_glow_player_col_visible : g_weebwarecfg.visuals_glow_player_col_hidden;
			else col = g_weebwarecfg.visuals_glow_player_col_visible;
			break;
		case 128:
			if (!g_weebwarecfg.visuals_glow_c4) continue;
			col = g_weebwarecfg.visuals_glow_c4_col;
			break;
		case 36:
			if (!g_weebwarecfg.visuals_glow_chicken) continue;
			col = g_weebwarecfg.visuals_glow_chicken_col;
			break;
		case -1:
			if (!g_weebwarecfg.visuals_glow_weapon)
				continue;

			col = g_weebwarecfg.visuals_glow_weapon_col;
			glowObject.m_flRed = col.r / 255.0f;
			glowObject.m_flGreen = col.g / 255.0f;
			glowObject.m_flBlue = col.b / 255.0f;
			glowObject.m_flAlpha = col.a / 255.0f;
			glowObject.m_bRenderWhenOccluded = true;
			glowObject.m_nGlowStyle = 0;
			glowObject.m_bFullBloomRender = false;
			continue;
		default:
			continue;
		}

		// only apply to enemy, chicken is enemy
		if (entity->m_iTeamNum() == local->m_iTeamNum() && !g_weebwarecfg.visuals_glow_team)
			continue;

		if (entity->m_iTeamNum() == local->m_iTeamNum())
			col = g_weebwarecfg.visuals_glow_team_col;

		glowObject.m_flRed = col.r / 255.0f;
		glowObject.m_flGreen = col.g / 255.0f;
		glowObject.m_flBlue = col.b / 255.0f;
		glowObject.m_flAlpha = col.a / 255.0f;
		glowObject.m_bRenderWhenOccluded = true;
		glowObject.m_nGlowStyle = 0;
		glowObject.m_bFullBloomRender = false;
	}
}