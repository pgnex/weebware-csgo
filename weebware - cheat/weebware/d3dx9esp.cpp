#include "Header.h"
#include "drawing.h"
#include "d3dx9esp.h"
#include "Legit.h"
#include <d3d9types.h>
#include "visuals_shared.h"

c_draw draw;

void reclass_test(c_base_entity* local);

void c_d3dxesp::d9esp_main(IDirect3DDevice9* pDevice) {
	static bool init = false;

	if (!init) {
		get_device(pDevice);
		draw.AddFont("Tahoma", 11, false, false);
		draw.AddFont("Tahoma", 18, false, false);
		draw.Reset();
		init = true;
	}
	
	// draw anything before any checks..
	water_mark();

	if (!g_weebware.g_engine->is_connected())
		return;

	if (!g_weebware.g_engine->is_in_game())
		return;

	display_backtrack();

	if (g_weebwarecfg.visuals_backtrack_dots) {

		for (size_t i = 0; i < g_accuracy.accuracy_records.size(); i++)
		{
			Vector w2s;

			if (g_maths.CRWorldToScreen(g_accuracy.accuracy_records[i].m_head, w2s))
			{
				g_accuracy.accuracy_records[i].m_w2s_head = w2s;
				g_accuracy.accuracy_records[i].m_has_w2s = true;
			}
		}
	}

	if (g_weebwarecfg.enable_visuals == 0)
		return;

	local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!local)
		return;

	//reclass_test(local);

	// on key visuals.. not sure why anyone would use but :shrug:
	if (g_weebwarecfg.enable_visuals == 2)
		if (!GetAsyncKeyState(g_weebwarecfg.enable_visuals_key))
			return;


	// hitmarker drawings
	//g_events.on_paint();

	// visuals not needed in ent loop
	draw_inaccuracy_circle();
	draw_sniper_crosshair();
	recoil_crosshair();

	spec_count = 0;
	for (int i = 1; i <= g_weebware.g_entlist->getmaxentities(); i++)
	{
		c_base_entity* ent = g_weebware.g_entlist->getcliententity(i);

		if (!ent || ent == nullptr)
			continue;

		
		spectator_list(ent);
		bomb_timer(ent);

		// anything past here must be a valid, living, player
		if (!ent->is_valid_player())
			continue;

		if (!g_weebwarecfg.visuals_dormant_esp)
			if (ent->is_dormant())
				continue;	


		if (!g_weebwarecfg.visuals_teammates && ent->m_iTeamNum() == local->m_iTeamNum())
			continue;

		// engine radar
		if (g_weebwarecfg.visuals_bspotted)
			*ent->b_spotted() = true;

		if (g_weebwarecfg.visuals_visible_only && !is_visible(local, ent))
			continue;

		w2s_player[i].boundary = calc_boundaries(ent);

		if (ent->is_dormant())
			w2s_player[i].boundary.dormant = true;


		if (local->m_iHealth() <= 0) {
			auto obv = g_weebware.g_entlist->getcliententityfromhandle(local->m_hObserverTarget());
			if (!obv)
				return;

			if (obv->EntIndex() == i)
				continue;
		}

		if (ent == local)
			continue;


		render_box(w2s_player[i].boundary, ent, is_visible(local, ent));
		render_box_corners(w2s_player[i].boundary, ent, is_visible(local, ent));
		render_skeleton(ent, is_visible(local, ent));
		render_health(w2s_player[i].boundary, ent);
		render_name(w2s_player[i].boundary, ent, is_visible(local, ent));
		render_ammo(w2s_player[i].boundary, ent);
		render_weapon(w2s_player[i].boundary, ent);

		//if (g_weebwarecfg.anime_model > 0)
		//	set_player_models(ent);
	}
}

void c_d3dxesp::water_mark() {

	if (!g_weebwarecfg.visuals_watermark)
		return;

	c_color col = c_color(g_weebwarecfg.water_mark_col);
	std::string watermark_text = ("weebware - " + g_weebware.g_user_name);
	std::transform(watermark_text.begin(), watermark_text.end(), watermark_text.begin(), ::tolower);

	draw.Text(watermark_text.c_str(), 5, 10, lefted, tahoma, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
}

void c_d3dxesp::draw_inaccuracy_circle() {

	//if (!g_weebwarecfg.visuals_inacc_circle)
	//	return;

	//c_weapon_info* weapon_data = g_weebware.g_weapon_system->GetWpnData(local->m_pActiveWeapon()->m_iItemDefinitionIndex());

	//if (!weapon_data)
	//	return;

	//// make sure it is a weapon
	//if (weapon_data->type < weapon_info_type::WEAPON_KNIFE || weapon_data->type > weapon_info_type::WEAPON_SNIPER)
	//	return;

	//c_color col = c_color(g_weebwarecfg.visuals_innacc_circle_col);

	//draw.Circle(draw.Screen.x_center, draw.Screen.y_center, weapon_data->spread * 200, 0, full, true, 24, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
}

void c_d3dxesp::render_health(s_boundaries bounds, c_base_entity* ent) {

	if (!g_weebwarecfg.visuals_health_bars)
		return;

	if (!bounds.has_w2s)
		return;

	float hp = (100.f - ent->m_iHealth()) / 100.f;

	int length = (hp * bounds.h);

	// render background
	draw.Line(bounds.x - 1, bounds.y - 4, bounds.x - 1, bounds.y + bounds.h, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0));
	draw.Line(bounds.x - 2, bounds.y - 4, bounds.x - 2, bounds.y + bounds.h, 1, false, D3DCOLOR_ARGB(50, 0, 0, 0));
	draw.Line(bounds.x - 3, bounds.y - 4, bounds.x - 3, bounds.y + bounds.h, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0));

	// render the line.
	// Calculate color

	c_color col = c_color(int(255.f - float(ent->m_iHealth()) * 2.55f), int(float(ent->m_iHealth()) * 2.55f), 0, 255);

	if (g_weebwarecfg.visuals_dormant_esp) {
		if (bounds.dormant) {
			col = (ent->m_iTeamNum() == local->m_iTeamNum()) ? c_color(g_weebwarecfg.visuals_dormant_col_team) : c_color(g_weebwarecfg.visuals_dormant_col);
		}
	}

	draw.Line(bounds.x - 2, (bounds.y + length) - 3, bounds.x - 2, bounds.y + bounds.h, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));

	if (ent->m_iHealth() < 100 && ent->m_iHealth() > 0) {
		draw.Text(std::to_string(ent->m_iHealth()).c_str(), bounds.x - 5, bounds.y + length, lefted, tahoma, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void c_d3dxesp::display_backtrack() {

	if (!g_weebwarecfg.visuals_backtrack_dots)
		return;

	Vector w2sParent, w2sChild;
	// Loop thru entities and get record to draw.
	if (g_weebwarecfg.visuals_backtrack_style == 1) {
		// best
		for (int i = 0; i < g_accuracy.m_best_record.bonecount; i++) {

			g_maths.CRWorldToScreen(g_accuracy.m_best_record.parent[i], w2sParent);
			g_maths.CRWorldToScreen(g_accuracy.m_best_record.child[i], w2sChild);

			c_color col = c_color(g_weebwarecfg.visuals_backtrack_col);

			draw.Line(w2sParent.x, w2sParent.y, w2sChild.x, w2sChild.y, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
		}
	}
	else {

		for (auto record : g_accuracy.accuracy_records)
		{
			if (g_weebwarecfg.visuals_backtrack_style == 0) { // time
				if (record.record_tick != g_accuracy.m_best_record.record_tick)
					continue;
			}
			else if (g_weebwarecfg.visuals_backtrack_style == 3)  // all for single target
				if (g_accuracy.m_best_record.index != record.index) {
					continue;
				}

			// best tick to draw skeleton
			for (int i = 0; i < record.bonecount; i++) {

				g_maths.CRWorldToScreen(record.parent[i], w2sParent);
				g_maths.CRWorldToScreen(record.child[i], w2sChild);

				c_color col = c_color(g_weebwarecfg.visuals_backtrack_col);

				draw.Line(w2sParent.x, w2sParent.y, w2sChild.x, w2sChild.y, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
			}
		}
	}
}

void c_d3dxesp::render_name(s_boundaries bounds, c_base_entity* ent, bool is_visible) {

	if (!g_weebwarecfg.visuals_name_esp)
		return;

	if (!bounds.has_w2s)
		return;

	s_player_info playerinfo;

	g_weebware.g_engine->get_player_info(ent->EntIndex(), &playerinfo);

	c_color draw_col;

	if (!(ent->m_iTeamNum() == local->m_iTeamNum())) {
		draw_col = is_visible ? c_color(g_weebwarecfg.visuals_name_esp_col_visible) : c_color(g_weebwarecfg.visuals_name_esp_col_hidden);
	}
	else {
		draw_col = is_visible ? c_color(g_weebwarecfg.team_visible_col) : c_color(g_weebwarecfg.team_hidden_col);
	}

	std::string player_name = playerinfo.name;
	int tw = draw.GetTextWidth(player_name.c_str(), tahoma);
	int th = draw.GetTextHeight(player_name.c_str(), tahoma);
	draw.Text(player_name.c_str(), (bounds.x + bounds.w / 2) - (tw / 2), (bounds.y - th) - 2, lefted, tahoma, D3DCOLOR_ARGB(draw_col.a, draw_col.r, draw_col.g, draw_col.b));
}

void c_d3dxesp::defusing_indicator(s_boundaries bounds, c_base_entity* ent) {

	if (!g_weebwarecfg.defusing_indicator)
		return;

	if (!bounds.has_w2s)
		return;

	if (ent->m_bIsDefusing()) {
		int tw = draw.GetTextWidth("Defusing", tahoma);
		draw.Text("Defusing", (bounds.x + bounds.w / 2) - (tw / 2), bounds.y - 20, lefted, tahoma, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void c_d3dxesp::render_weapon(s_boundaries bounds, c_base_entity* ent) {

	if (!g_weebwarecfg.visuals_weapon_esp)
		return;

	if (!bounds.has_w2s)
		return;

	c_weapon_info* weapon_data = g_weebware.g_weapon_system->GetWpnData(ent->m_pActiveWeapon()->m_iItemDefinitionIndex());

	if (!weapon_data)
		return;

	std::string weapon_name = weapon_data->name;
	
	int tw = draw.GetTextWidth(weapon_name.c_str(), tahoma);
	int th = draw.GetTextHeight(weapon_name.c_str(), tahoma);
	draw.Text(weapon_name.c_str(), (bounds.x + bounds.w / 2) - (tw / 2), (bounds.y + bounds.h + th) + 2, lefted, tahoma, D3DCOLOR_ARGB(255, 255, 255, 255));

}

//void reclass_test(c_base_entity* local) {
//	std::cout << g_weebware.g_weapon_system->GetWpnData(local->m_pActiveWeapon()->m_iItemDefinitionIndex()) << std::endl;
//}



void c_d3dxesp::render_ammo(s_boundaries bounds, c_base_entity* ent) {

	if (!g_weebwarecfg.visuals_ammo_esp)
		return;

	if (!bounds.has_w2s)
		return;

	c_weapon_info* weapon_data = g_weebware.g_weapon_system->GetWpnData(ent->m_pActiveWeapon()->m_iItemDefinitionIndex());

	if (!weapon_data)
		return;

	int max_ammo = weapon_data->max_ammo;
//	int current_ammo = weapon_data->m_iAmmo;

	std::string out = "??/" + std::to_string(max_ammo);


	if (weapon_data->type == weapon_info_type::WEAPON_KNIFE)
		out = "Infinite";

	int offset = 0;

	int tw = draw.GetTextWidth(out.c_str(), tahoma);
	int th = draw.GetTextHeight(out.c_str(), tahoma);

	if (g_weebwarecfg.visuals_weapon_esp)
		offset = th * 2;

	draw.Text(out.c_str(), (bounds.x + bounds.w / 2) - (tw / 2), (bounds.y + bounds.h + offset) + 2, lefted, tahoma, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void c_d3dxesp::draw_sniper_crosshair() {

	if (!g_weebwarecfg.visuals_sniper_crosshair)
		return;

	c_weapon_info* weapon_data = g_weebware.g_weapon_system->GetWpnData(local->m_pActiveWeapon()->m_iItemDefinitionIndex());

	if (!weapon_data)
		return;

	// make sure it is a weapon
	if (weapon_data->type < weapon_info_type::WEAPON_KNIFE || weapon_data->type > weapon_info_type::WEAPON_SNIPER)
		return;

	// make sure its a sniper 
	if (!weapon_data->type == weapon_info_type::WEAPON_SNIPER)
		return;


	c_color col = g_weebwarecfg.visuals_sniper_crosshair_col;
	int cx = draw.Screen.x_center;
	int cy = draw.Screen.y_center;
	draw.Line(cx - 8, cy - 0, cx + 8, cy + 0, 2, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
	draw.Line(cx + 0, cy - 8, cx - 0, cy + 8, 2, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
}

void c_d3dxesp::recoil_crosshair() {

	if (!g_weebwarecfg.visuals_recoil_crosshair)
		return;

	Vector view_angles;
	g_weebware.g_engine->get_view_angles(view_angles);

	view_angles += local->m_aimPunchAngle() * 2;

	Vector forward_vec;
	g_maths.qangle_vector(view_angles, forward_vec);
	forward_vec *= 10000;

	Vector start = local->get_vec_eyepos();
	Vector end = start + forward_vec, end_screen;

	c_color col = g_weebwarecfg.visuals_recoil_crosshair_col;

	if (!g_maths.CRWorldToScreen(end, end_screen))
		return;

	draw.Line(end_screen.x - 10, end_screen.y, end_screen.x + 10, end_screen.y, 2, true, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
	draw.Line(end_screen.x, end_screen.y - 10, end_screen.x, end_screen.y + 10, 2, true, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));

}

void c_d3dxesp::bomb_timer(c_base_entity* ent) {

	if (!strstr(ent->get_client_class()->m_networkedname, "CPlantedC4"))
		return;

	if (!g_weebwarecfg.visuals_bomb_timer)
		return;

	float remaining = reinterpret_cast<c_bomb*>(ent)->get_blow_time() - g_weebware.g_global_vars->curtime;

	// So c4 is still going!
	if (reinterpret_cast<c_bomb*>(ent)->is_ticking() && remaining > 0.f) {

		int offset_y = 15.f;

		if (g_weebwarecfg.visuals_watermark) {
			offset_y += 35.f;
		}

		int Damage = 500;
		float BombRadius = 1750;

		auto vecToTarget = ent->get_vec_eyepos() - local->get_vec_eyepos();

		// + 1 to account for imprecision, also magic number btw.

		float predicted_damage = 500.f * exp(-((vecToTarget.size() * vecToTarget.size()) / 680555.582778f));

		if ((local->m_ArmorValue() > 0)) {

			predicted_damage *= 0.5f;

			if ((predicted_damage / 2) > local->m_ArmorValue())
			{
				predicted_damage += (local->m_ArmorValue() * 2);
			}
		}
		predicted_damage += 1.f;

		predicted_damage = ceilf(predicted_damage);


		Vector bomb_pos = ent->getrenderorigin();
		Vector screen_pos;

		if (!g_maths.CRWorldToScreen(bomb_pos, screen_pos))
			return;

		char time_text[55];
		char damage_text[55];

		sprintf(damage_text, "Damage: %.0f", floor(predicted_damage));
		sprintf(time_text, "Time: %.2fs", remaining);

		int tw1, tw2;

		tw1 = draw.GetTextWidth(time_text, tahoma_large);
		tw2 = draw.GetTextWidth(damage_text, tahoma_large);


		draw.Text(time_text, screen_pos.x - (tw1 / 2), screen_pos.y - 40, lefted, tahoma_large, predicted_damage < local->m_iHealth() ? D3DCOLOR_ARGB(255, 0, 255, 0) : D3DCOLOR_ARGB(255, 255, 0, 0));
		draw.Text(damage_text, screen_pos.x - (tw2 / 2), screen_pos.y - 25, lefted, tahoma_large, predicted_damage < local->m_iHealth() ? D3DCOLOR_ARGB(255, 0, 255, 0) : D3DCOLOR_ARGB(255, 255, 0, 0));
	}
}

void c_d3dxesp::render_box(s_boundaries bounds, c_base_entity* ent, bool is_visible) {

	if (!g_weebwarecfg.visuals_bounding_box)
		return;

	if (!bounds.has_w2s)
		return;

	c_color col;
	if (!(ent->m_iTeamNum() == local->m_iTeamNum())) {
		col = is_visible ? c_color(g_weebwarecfg.visuals_bounding_col_visible) : c_color(g_weebwarecfg.visuals_bounding_col_hidden);
	}
	else {
		col = is_visible ? c_color(g_weebwarecfg.team_visible_col) : c_color(g_weebwarecfg.team_hidden_col);
	}


	if (g_weebwarecfg.visuals_dormant_esp) {
		if (bounds.dormant) {
			c_color col = is_visible ? c_color(g_weebwarecfg.visuals_dormant_col_team) : c_color(g_weebwarecfg.visuals_dormant_col);
		}
	}

	// outline
	draw.BoxOutlined(bounds.x + 1, bounds.y - 3, bounds.w - 4, bounds.h + 2, D3DCOLOR_ARGB(60, 0, 0, 0));
	// box
	draw.BoxOutlined(bounds.x + 2, bounds.y - 2, bounds.w - 6, bounds.h, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
}

void c_d3dxesp::render_box_corners(s_boundaries bounds, c_base_entity* ent, bool is_visible) {

	if (!g_weebwarecfg.visuals_corner_box)
		return;

	if (!bounds.has_w2s)
		return;

	c_color col;
	if (!(ent->m_iTeamNum() == local->m_iTeamNum())) {
		col = is_visible ? c_color(g_weebwarecfg.visuals_corner_col_visible) : c_color(g_weebwarecfg.visuals_corner_col_hidden);
	}
	else {
		col = is_visible ? c_color(g_weebwarecfg.team_visible_col) : c_color(g_weebwarecfg.team_hidden_col);
	}


	int x = bounds.x + 2;
	int y = bounds.y - 2;
	int w = bounds.w - 6;
	int h = bounds.h;

	float line_w = (w / 5);
	float line_h = (h / 6);
	float line_t = 1;

	// outline stuff
	draw.Line(x - line_t, y - line_t, x + line_w, y - line_t, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0));
	draw.Line(x - line_t, y - line_t, x - line_t, y + line_h, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0));
	draw.Line(x - line_t, y + h - line_h, x - line_t, y + h + line_t, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0));; //bot left
	draw.Line(x - line_t, y + h + line_t, x + line_w, y + h + line_t, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0));
	draw.Line(x + w - line_w, y - line_t, x + w + line_t, y - line_t, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0)); // top right
	draw.Line(x + w + line_t, y - line_t, x + w + line_t, y + line_h, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0));
	draw.Line(x + w + line_t, y + h - line_h, x + w + line_t, y + h + line_t, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0)); // bot right
	draw.Line(x + w - line_w, y + h + line_t, x + w + line_t, y + h + line_t, 1, false, D3DCOLOR_ARGB(255, 0, 0, 0));;


	// inner lines
	draw.Line(x, y, x, y + line_h, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b)); //top left
	draw.Line(x, y, x + line_w, y, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
	draw.Line(x + w - line_w, y, x + w, y, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b)); //top right
	draw.Line(x + w, y, x + w, y + line_h, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
	draw.Line(x, y + h - line_h, x, y + h, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b)); //bot left
	draw.Line(x, y + h, x + line_w, y + h, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
	draw.Line(x + w - line_w, y + h, x + w, y + h, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b)); //bot right
	draw.Line(x + w, y + h - line_h, x + w, y + h, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));

}

void c_d3dxesp::render_skeleton(c_base_entity* ent, bool is_visible) {

	if (!g_weebwarecfg.visuals_skeleton)
		return;

	const model_t* model;
	studiohdr_t* hdr;
	mstudiobone_t* bone;
	Vector        bone_world_pos, parent_bone_world_pos, bone_screen_pos, parent_bone_screen_pos;

	model = ent->getmodel();
	if (!model)
		return;

	hdr = g_weebware.g_model_info->getstudiomodel(model);
	if (!hdr)
		return;


	c_color col;
	if (!(ent->m_iTeamNum() == local->m_iTeamNum())) {
		col = is_visible ? c_color(g_weebwarecfg.visuals_skeleton_col_visible) : c_color(g_weebwarecfg.visuals_skeleton_col_hidden);
	}
	else {
		col = is_visible ? c_color(g_weebwarecfg.team_visible_col) : c_color(g_weebwarecfg.team_hidden_col);
	}

	for (int i = 0; i < hdr->numbones; ++i) {
		bone = hdr->GetBone(i);

		if (bone && (bone->flags & 0x00000100) && bone->parent != -1) {

			bone_world_pos = ent->get_bone(i);
			parent_bone_world_pos = ent->get_bone(bone->parent);

			int iChestBone = 6;
			Vector vBreastBone;
			Vector vUpperDirection = ent->get_bone(iChestBone + 1) - ent->get_bone(iChestBone);
			vBreastBone = ent->get_bone(iChestBone) + vUpperDirection / 2;
			Vector vDeltaChild = bone_world_pos - vBreastBone;
			Vector vDeltaParent = parent_bone_world_pos - vBreastBone;

			if ((vDeltaParent.size() < 9 && vDeltaChild.size() < 9))
				parent_bone_world_pos = vBreastBone;

			if (i == iChestBone - 1)
				bone_world_pos = vBreastBone;

			if (abs(vDeltaChild.z) < 5 && (vDeltaParent.size() < 5 && vDeltaChild.size() < 5) || i == iChestBone)
				continue;

			g_maths.CRWorldToScreen(bone_world_pos, bone_screen_pos);
			g_maths.CRWorldToScreen(parent_bone_world_pos, parent_bone_screen_pos);

			draw.Line(bone_screen_pos.x, bone_screen_pos.y, parent_bone_screen_pos.x, parent_bone_screen_pos.y, 1, false, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
		}
	}
}

void c_d3dxesp::spectator_list(c_base_entity* ent) {

	if (!g_weebwarecfg.spec_list)
		return;

	if (!local)
		return;

	if (!local->is_valid_player())
		return;

	if (ent->m_iHealth() > 0)
		return;

	if (ent->is_dormant())
		return;

	if (ent == local)
		return;

	if (ent->get_client_class()->m_ClassID != class_ids::ccsplayer)
		return;

	s_player_info local_info;
	g_weebware.g_engine->get_player_info(local->EntIndex(), &local_info);

	s_player_info ent_info;
	g_weebware.g_engine->get_player_info(ent->EntIndex(), &ent_info);

	if (ent_info.ishltv)
		return;

	auto obv = g_weebware.g_entlist->getcliententityfromhandle(ent->m_hObserverTarget());

	if (!obv)
		return;

	s_player_info target_info;
	g_weebware.g_engine->get_player_info(obv->EntIndex(), &target_info);

	if (strcmp(local_info.name, target_info.name) == 0) {
		spec_count++;

		int sw, sh, tw, th;
		g_weebware.g_engine->get_screen_dimensions(sw, sh);

		c_color col = c_color(255, 255, 255, 255);
		std::string spec_name = ent_info.name;

		tw = draw.GetTextWidth(spec_name.c_str(), tahoma_large);
		th = draw.GetTextHeight(spec_name.c_str(), tahoma_large);
		std::transform(spec_name.begin(), spec_name.end(), spec_name.begin(), ::tolower);
		draw.Text(spec_name.c_str(), (sw - tw) - 5, (spec_count * th) + 2, lefted, tahoma_large, D3DCOLOR_ARGB(col.a, col.r, col.g, col.b));
	}

}

void c_d3dxesp::on_lost_device() {
	if (draw.Font()) draw.OnLostDevice();
}

void c_d3dxesp::get_device(IDirect3DDevice9* pDevice) {
	draw.GetDevice(pDevice);
}

void c_d3dxesp::d3dx_reset() {
	draw.FontReset();
	draw.Reset();
}

