#include "esp.h"
#include "drawing.h"
#include "Legit.h"
#include "paint_traverse.h"
#include "events.h"

c_esp g_esp;
FeatureFuncs g_event_features;
bool has_esp_init = false;

void c_esp::esp_main()
{
	water_mark();

	local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
		return;

	if (g_weebware.g_engine->is_taking_screenshot() && g_weebwarecfg.screenshot_proof)
		return;

	if (g_weebwarecfg.visuals_hitmarkers) g_event_features.on_paint();

	if (!local)
		return;

	// call things here if no ent loop needed 

	draw_inaccuracy_circle();
	draw_fov_circle();
	draw_crosshair();
	recoil_crosshair();

	for (int i = 1; i <= g_weebware.g_entlist->getmaxentities(); i++)
	{
		c_base_entity* ent = g_weebware.g_entlist->getcliententity(i);

		if (!ent || ent == nullptr)
			continue;

		// check if bomb timer is enabled, if it is check for entity
		if (g_weebwarecfg.visuals_bomb_timer && strstr(ent->get_client_class()->m_networkedname, "CPlantedC4"))
			bomb_timer(ent);

		if (!ent || ent->m_iHealth() <= 0 || ent->get_client_class()->m_ClassID != 40) {
			continue;
		}

		if (g_weebwarecfg.anime_model > 0)
			set_models(ent);
	}


	if (g_weebware.menu_opened) {
		int s, h;
		g_weebware.g_engine->get_screen_dimensions(s, h);
		g_weebware.g_surface->drawsetcolor(0, 0, 0, 185);
		g_weebware.g_surface->drawfilledrect(0, 0, s, h);
	}

	if (g_weebwarecfg.enable_visuals == 0)
		return;

	if (g_weebwarecfg.enable_visuals == 2)
		if (!GetAsyncKeyState(g_weebwarecfg.enable_visuals_key))
			return;


	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game()) {

		draw_inaccuracy_circle();
		display_backtrack();

		if (g_weebwarecfg.visuals_backtrack_dots) {

			for (size_t i = 0; i < g_accuracy.accuracy_records.size(); i++)
			{
				Vector w2s;

				if (g_maths.world_to_screen(g_accuracy.accuracy_records[i].m_head, w2s))
				{
					g_accuracy.accuracy_records[i].m_w2s_head = w2s;
					g_accuracy.accuracy_records[i].m_has_w2s = true;
				}
			}

		}

	}

	for (int i = 1; i <= g_weebware.g_entlist->getmaxentities(); i++)
	{
		c_base_entity* ent = g_weebware.g_entlist->getcliententity(i);

		if (!ent || ent == nullptr)
			continue;

		if (!ent || ent->m_iHealth() <= 0 || ent->get_client_class()->m_ClassID != 40) {
			continue;
		}

		if (!g_weebwarecfg.visuals_dormant_esp) {
			if (ent->is_dormant()) {
				continue;
			}
		}

		if (!g_weebwarecfg.visuals_teammates && ent->m_iTeamNum() == local->m_iTeamNum()) {
			continue;
		}

		if (ent == local) {
			continue;
		}

		if (g_weebwarecfg.visuals_bspotted) {
			*ent->b_spotted() = true;
		}

		if (g_weebwarecfg.visuals_visible_only && !is_visible(local, ent)) {
			continue;
		}

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

		render_box(w2s_player[i].boundary, ent, is_visible(local, ent));
		render_box_corners(w2s_player[i].boundary, ent, is_visible(local, ent));
		render_health(w2s_player[i].boundary, ent, ent->m_iTeamNum() == local->m_iTeamNum());
		render_name(w2s_player[i].boundary, ent, is_visible(local, ent));
		render_weapon(w2s_player[i].boundary, ent);
		render_ammo(w2s_player[i].boundary, ent);
		render_skeleton(ent, is_visible(local, ent));

	}
}


void c_esp::calc_w2svalues()
{
	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
	{
		local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local()); // getting localplayer

		if (local)
		{

			for (int i = 1; i <= g_weebware.g_engine->get_max_clients(); i++)
			{
				c_base_entity* ent = g_weebware.g_entlist->getcliententity(i);

				if (!ent || ent->m_iHealth() <= 0 || ent->get_client_class()->m_ClassID != 40) {
					continue;
				}

				if (!g_weebwarecfg.visuals_dormant_esp) {
					if (ent->is_dormant()) {
						continue;
					}
				}

				if (!g_weebwarecfg.visuals_teammates && ent->m_iTeamNum() == local->m_iTeamNum())
				{
					continue;
				}

				if (i == local->EntIndex())
				{
					continue;
				}

				if (g_weebwarecfg.visuals_visible_only && !is_visible(local, ent)) {
					continue;
				}
			}
		}


	}
}

void c_esp::set_models(c_base_entity* ent) {

	switch (g_weebwarecfg.anime_model) {
	case 1:
		if (ent->m_iTeamNum() == local->m_iTeamNum())
			*ent->m_nModelIndex() = g_weebware.g_model_info->getmodelindex("models/player/custom_player/caleon1/reinakousaka/reina_blue.mdl");
		else
			*ent->m_nModelIndex() = g_weebware.g_model_info->getmodelindex("models/player/custom_player/caleon1/reinakousaka/reina_red.mdl");
		break;
	case 2:
		*ent->m_nModelIndex() = g_weebware.g_model_info->getmodelindex("models/player/custom_player/voikanaa/mirainikki/gasaiyono.mdl");
		break;
	}
}

void c_esp::recoil_crosshair() {

	if (!g_weebwarecfg.visuals_recoil_crosshair)
		return;

	c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	if (!local)
		return;

	if (!local->is_valid_player())
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

	if (g_maths.world_to_screen(end, end_screen)) {
		g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);
		g_weebware.g_surface->drawline(end_screen.x - 10, end_screen.y, end_screen.x + 10, end_screen.y);
		g_weebware.g_surface->drawline(end_screen.x, end_screen.y - 10, end_screen.x, end_screen.y + 10);
	}
}

void c_esp::water_mark() {

	if (!g_weebwarecfg.visuals_watermark)
		return;

	g_paint_traverse.draw_water_mark();
}

// Returns the center of a hitbox
// Could have globalised legitbots one and used it
Vector c_esp::center_hitbox(c_base_entity* ent, int id)
{
	if (!ent)
	{
		return Vector(0, 0, 0);
	}

	matrix3x4_t matrix[128];

	if (!ent->setup_bones(matrix, 128, 0x00000100, GetTickCount64()))
	{
		return Vector(0, 0, 0);
	}

	const model_t* mod = ent->get_model();

	if (!mod)
	{
		return Vector(0, 0, 0);
	}

	studiohdr_t* hdr = g_weebware.g_model_info->getstudiomodel(mod);

	if (!hdr)
	{
		return Vector(0, 0, 0);
	}

	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

	if (!set)
	{
		return Vector(0, 0, 0);
	}

	mstudiobbox_t* hitbox = set->pHitbox(id);

	if (!hitbox)
	{
		return Vector(0, 0, 0);
	}
	Vector vMin, vMax, vCenter;

	g_maths.VectorTransform2(hitbox->bbmin, matrix[hitbox->bone], vMin);
	g_maths.VectorTransform2(hitbox->bbmax, matrix[hitbox->bone], vMax);

	vCenter = (vMin + vMax) * 0.5f;

	return vCenter;
}


s_boundaries c_esp::calc_boundaries(c_base_entity* Entity)
{
	s_boundaries result;

	Vector head = center_hitbox(Entity, (int)csgohitboxid::head);
	Vector body = center_hitbox(Entity, (int)csgohitboxid::pelvis);
	Vector foot = center_hitbox(Entity, (int)csgohitboxid::left_foot);

	Vector origin = head;

	// crouching flag not fast enough.

	if (body.z - foot.z > 30)
	{
		origin.z -= 72;
	}
	else
	{
		origin.z -= 52;
	}

	Vector ScreenTop, ScreenBottom;

	if (g_maths.world_to_screen(head, ScreenTop) && g_maths.world_to_screen(origin, ScreenBottom))
	{
		result.y = ScreenTop.y - ((ScreenBottom.y - ScreenTop.y) / 8);
		result.h = ScreenBottom.y - ScreenTop.y + ((ScreenBottom.y - ScreenTop.y) / 5);
		result.x = ScreenTop.x - ((ScreenBottom.y - ScreenTop.y) / 3);
		result.w = ((ScreenBottom.y - ScreenTop.y) * 0.7);
		result.has_w2s = true;
	}
	else {
		result.has_w2s = false;
	}
	return result;
}

void c_esp::bomb_timer(c_base_entity* ent) {
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

		char timer_s[55];

		sprintf(timer_s, "Time till explosion: %.2f\nDamage: %.0f", remaining, floor(predicted_damage));

		g_paint_traverse.draw_string(g_weebware.tahoma_font, 5, offset_y, predicted_damage < local->m_iHealth() ? c_color(0, 255, 0, 255) : c_color(255, 0, 0, 255), 0, timer_s);

		// Okay now lets check if we have defuse on the bomb..
		// m_flDefuseCountDown // interesting netvar let see what it returns...

	}
}


void c_esp::render_box_corners(s_boundaries bounds, c_base_entity* ent, bool is_visible) {

	if (!g_weebwarecfg.visuals_corner_box)
		return;

	c_color col;
	if (!(ent->m_iTeamNum() == local->m_iTeamNum())) {
		col = is_visible ? c_color(g_weebwarecfg.visuals_corner_col_visible) : c_color(g_weebwarecfg.visuals_corner_col_hidden);
	}
	else {
		col = is_visible ? c_color(g_weebwarecfg.team_visible_col) : c_color(g_weebwarecfg.team_hidden_col);
	}


	int x = bounds.x;
	int y = bounds.y;
	int w = bounds.w;
	int h = bounds.h;

	float line_w = (w / 5);
	float line_h = (h / 6);
	float line_t = 1;

	g_weebware.g_surface->drawsetcolor(0, 0, 0, 255);
	g_weebware.g_surface->drawline(x - line_t, y - line_t, x + line_w, y - line_t); //top left
	g_weebware.g_surface->drawline(x - line_t, y - line_t, x - line_t, y + line_h);
	g_weebware.g_surface->drawline(x - line_t, y + h - line_h, x - line_t, y + h + line_t); //bot left
	g_weebware.g_surface->drawline(x - line_t, y + h + line_t, x + line_w, y + h + line_t);
	g_weebware.g_surface->drawline(x + w - line_w, y - line_t, x + w + line_t, y - line_t); // top right
	g_weebware.g_surface->drawline(x + w + line_t, y - line_t, x + w + line_t, y + line_h);
	g_weebware.g_surface->drawline(x + w + line_t, y + h - line_h, x + w + line_t, y + h + line_t); // bot right
	g_weebware.g_surface->drawline(x + w - line_w, y + h + line_t, x + w + line_t, y + h + line_t);

	g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);
	g_weebware.g_surface->drawline(x, y, x, y + line_h);//top left
	g_weebware.g_surface->drawline(x, y, x + line_w, y);
	g_weebware.g_surface->drawline(x + w - line_w, y, x + w, y); //top right
	g_weebware.g_surface->drawline(x + w, y, x + w, y + line_h);
	g_weebware.g_surface->drawline(x, y + h - line_h, x, y + h); //bot left
	g_weebware.g_surface->drawline(x, y + h, x + line_w, y + h);
	g_weebware.g_surface->drawline(x + w - line_w, y + h, x + w, y + h);//bot right
	g_weebware.g_surface->drawline(x + w, y + h - line_h, x + w, y + h);
}


void c_esp::render_box(s_boundaries bounds, c_base_entity* ent, bool is_visible)
{
	if (!g_weebwarecfg.visuals_bounding_box)
		return;

	c_color col;
	if (!(ent->m_iTeamNum() == local->m_iTeamNum())) {
		col = is_visible ? c_color(g_weebwarecfg.visuals_bounding_col_visible) : c_color(g_weebwarecfg.visuals_bounding_col_hidden);
	}
	else {
		col = is_visible ? c_color(g_weebwarecfg.team_visible_col) : c_color(g_weebwarecfg.team_hidden_col);
	}

	// Box
	g_weebware.g_surface->drawsetcolor(0, 0, 0, 60);

	g_weebware.g_surface->drawoutlinedrect(bounds.x - 1, bounds.y - 1, bounds.w + 2 + bounds.x - 1, bounds.h + 2 + bounds.y - 1);

	if (g_weebwarecfg.visuals_dormant_esp) {

		if (bounds.dormant) {

			c_color col = is_visible ? c_color(g_weebwarecfg.visuals_dormant_col_team) : c_color(g_weebwarecfg.visuals_dormant_col);
		}

	}
	g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);

	g_weebware.g_surface->drawoutlinedrect(bounds.x, bounds.y, bounds.w + bounds.x, bounds.h + bounds.y);

}

void c_esp::render_health(s_boundaries bounds, c_base_entity* ent, bool is_team)
{
	if (!g_weebwarecfg.visuals_health_bars)
		return;

	if (!bounds.has_w2s)
		return;

	float hp = (100.f - ent->m_iHealth()) / 100.f;

	int length = (hp * bounds.h);

	// render the full black line.
	g_weebware.g_surface->drawsetcolor(0, 0, 0, 255);
	g_weebware.g_surface->drawline(bounds.x - 5, bounds.y, bounds.x - 5, bounds.y + bounds.h);
	g_weebware.g_surface->drawsetcolor(0, 0, 0, 50);
	g_weebware.g_surface->drawline(bounds.x - 6, bounds.y - 1, bounds.x - 6, bounds.y + bounds.h + 1);
	g_weebware.g_surface->drawline(bounds.x - 4, bounds.y - 1, bounds.x - 4, bounds.y + bounds.h + 1);

	// render the line.
	// Calculate color

	c_color col = c_color(int(255.f - float(ent->m_iHealth()) * 2.55f), int(float(ent->m_iHealth()) * 2.55f), 0, 255);

	if (g_weebwarecfg.visuals_dormant_esp) {
		if (bounds.dormant) {
			col = is_team ? c_color(g_weebwarecfg.visuals_dormant_col_team) : c_color(g_weebwarecfg.visuals_dormant_col);
		}
	}

	g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);

	g_weebware.g_surface->drawline(bounds.x - 5, bounds.y + length, bounds.x - 5, bounds.y + bounds.h);

	if (ent->m_iHealth() < 100 && ent->m_iHealth() > 0) {

		g_paint_traverse.draw_string(g_weebware.tahoma_font, bounds.x - 5, bounds.y + length, c_color(255, 255, 255, 255), 0, (char*)std::to_string(ent->m_iHealth()).c_str());

	}
}


void c_esp::render_ammo(s_boundaries bounds, c_base_entity* ent) {

	if (!g_weebwarecfg.visuals_ammo_esp)
		return;

	if (!ent)
		return;

	c_basecombat_weapon* weapon = ent->m_pActiveWeapon();

	if (!weapon)
		return;

	int max_ammo = weapon->m_iPrimaryReserveAmmoCount();
	int current_ammo = weapon->Clip1();

	wchar_t buf[128];
	std::string out = std::to_string(current_ammo) + "/" + std::to_string(max_ammo);

	if (!weapon->is_firearm())
		out = "N/A";

	int offset = 10;

	if (g_weebwarecfg.visuals_weapon_esp)
		offset = 22;

	if (MultiByteToWideChar(CP_UTF8, 0, out.c_str(), -1, buf, 128) > 0) {

		int tw, th;
		g_weebware.g_surface->gettextsize(g_weebware.tahoma_font, buf, tw, th);

		if (bounds.has_w2s)
			g_paint_traverse.draw_string(g_weebware.tahoma_font, (bounds.x + bounds.w / 2) - (tw / 2), (bounds.y + bounds.h) + offset, c_color(g_weebwarecfg.visuals_ammo_esp_col), 0, out.c_str());
	}
}

void c_esp::render_weapon(s_boundaries bounds, c_base_entity* ent) {

	if (!g_weebwarecfg.visuals_weapon_esp)
		return;

	if (!ent)
		return;

	c_basecombat_weapon* weapon = ent->m_pActiveWeapon();

	if (!weapon)
		return;

	wchar_t buf[128];
	std::string weapon_name = weapon->get_weapon_name_from_id();

	if (MultiByteToWideChar(CP_UTF8, 0, weapon_name.c_str(), -1, buf, 128) > 0) {

		int tw, th;
		g_weebware.g_surface->gettextsize(g_weebware.tahoma_font, buf, tw, th);

		if (bounds.has_w2s)
			g_paint_traverse.draw_string(g_weebware.tahoma_font, (bounds.x + bounds.w / 2) - (tw / 2), (bounds.y + bounds.h) + 10, c_color(g_weebwarecfg.visuals_weapon_esp_col), 0, weapon_name.c_str());
	}
}

void c_esp::render_name(s_boundaries bounds, c_base_entity* ent, bool is_visible)
{
	if (!g_weebwarecfg.visuals_name_esp)
		return;

	if (!ent)
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

	wchar_t buf[128];
	std::string player_name = playerinfo.name;

	if (MultiByteToWideChar(CP_UTF8, 0, playerinfo.name, -1, buf, 128) > 0)
	{
		int tw, th;
		g_weebware.g_surface->gettextsize(g_weebware.tahoma_font, buf, tw, th);

		if (bounds.has_w2s)
			g_paint_traverse.draw_string(g_weebware.tahoma_font, (bounds.x + bounds.w / 2) - (tw / 2), bounds.y - 5, draw_col, 0, player_name.c_str());
	}

}

void c_esp::render_skeleton(c_base_entity* ent, bool is_visible) {

	if (!g_weebwarecfg.visuals_skeleton)
		return;

	const model_t *model;
	studiohdr_t	  *hdr;
	matrix3x4_t	  bone_matrix[128];
	mstudiobone_t *bone;
	int			  parent_bone;
	Vector        bone_world_pos, parent_bone_world_pos, bone_screen_pos, parent_bone_screen_pos;

	model = ent->getmodel();
	if (!model)
		return;

	hdr = g_weebware.g_model_info->getstudiomodel(model);
	if (!hdr)
		return;

	ent->setupbones(bone_matrix, 128, 0x00000100, 0.f);

	c_color col;
	if (!(ent->m_iTeamNum() == local->m_iTeamNum())) {
		col = is_visible ? c_color(g_weebwarecfg.visuals_skeleton_col_visible) : c_color(g_weebwarecfg.visuals_skeleton_col_hidden);
	}
	else {
		col = is_visible ? c_color(g_weebwarecfg.team_visible_col) : c_color(g_weebwarecfg.team_hidden_col);
	}

	for (int i = 0; i < hdr->numbones; ++i) {
		bone = hdr->GetBone(i);

		if (!bone)
			continue;

		if (!bone->flags & 0x00000100)
			continue;

		parent_bone = bone->parent;
		if (parent_bone == -1)
			continue;

		bone_world_pos = bone_matrix[i].at(3);
		parent_bone_world_pos = bone_matrix[parent_bone].at(3);


		if (g_maths.world_to_screen(bone_world_pos, bone_screen_pos) && g_maths.world_to_screen(parent_bone_world_pos, parent_bone_screen_pos)) {
			g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);
			g_weebware.g_surface->drawline(bone_screen_pos.x, bone_screen_pos.y, parent_bone_screen_pos.x, parent_bone_screen_pos.y);
		}
	}
}

struct player_esp_info
{
	std::string name;
	bool has_helmet;
	int health;
	int armor;
	int ammo;
};

void c_esp::draw_crosshair() {

	if (!g_weebwarecfg.visuals_sniper_crosshair)
		return;

	c_basecombat_weapon* weapon = local->m_pActiveWeapon();

	if (!weapon || weapon == nullptr)
		return;

	if (!weapon->is_firearm())
		return;

	if (!weapon->is_autosniper() && !weapon->is_awp() && !weapon->is_scout())
		return;

	int x, y;
	g_weebware.g_engine->get_screen_dimensions(x, y);

	int cx = x / 2;
	int cy = y / 2;

	c_color col = g_weebwarecfg.visuals_sniper_crosshair_col;

	g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);
	g_weebware.g_surface->drawline(cx - 8, cy - 0, cx + 8, cy + 0);
	g_weebware.g_surface->drawline(cx + 0, cy - 8, cx - 0, cy + 8);

}

void c_esp::draw_fov_circle() {

	if (!g_weebwarecfg.visuals_fov_circle)
		return;

	if (!local)
		return;

	if (local->m_iHealth() <= 0)
		return;

	int x, y;
	float fov;

	fov = g_weebwarecfg.legit_cfg[g_legitbot.get_config_index()].maximum_fov;
	g_weebware.g_engine->get_screen_dimensions(x, y);
	c_color col = c_color(g_weebwarecfg.visuals_innacc_circle_col);

	float radius = tanf(DEG2RAD(fov) / 2) / tanf(DEG2RAD(g_hooking.o_vm()) / 2) * x;

	g_weebware.g_surface->drawcoloredcircle(x / 2, y / 2, radius / 2, col.r, col.g, col.b, col.a);
}

void c_esp::draw_inaccuracy_circle()
{
	if (!g_weebwarecfg.visuals_inacc_circle)
		return;

	auto weapon = local->m_pActiveWeapon();

	if (!weapon)
		return;

	if (!weapon->is_firearm())
		return;

	int x, y;
	g_weebware.g_engine->get_screen_dimensions(x, y);
	c_color col = c_color(g_weebwarecfg.visuals_innacc_circle_col);

	g_weebware.g_surface->drawcoloredcircle(x / 2, y / 2, weapon->Get_Innacuracy() * 200, col.r, col.g, col.b, col.a);
}

void c_esp::display_backtrack()
{
	if (!g_weebwarecfg.visuals_backtrack_dots)
		return;

	Vector w2sParent, w2sChild;
	// Loop thru entities and get record to draw.
	if (g_weebwarecfg.visuals_backtrack_style == 1) {
		// best
		for (int i = 0; i < g_accuracy.m_best_record.bonecount; i++) {

			g_maths.world_to_screen(g_accuracy.m_best_record.parent[i], w2sParent);
			g_maths.world_to_screen(g_accuracy.m_best_record.child[i], w2sChild);

			c_color col = c_color(g_weebwarecfg.visuals_backtrack_col);

			g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);

			g_weebware.g_surface->drawline(w2sParent.x, w2sParent.y, w2sChild.x, w2sChild.y);
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

				g_maths.world_to_screen(record.parent[i], w2sParent);
				g_maths.world_to_screen(record.child[i], w2sChild);

				c_color col = c_color(g_weebwarecfg.visuals_backtrack_col);

				g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);

				g_weebware.g_surface->drawline(w2sParent.x, w2sParent.y, w2sChild.x, w2sChild.y);
			}
		}
	}
}

bool c_esp::is_visible(c_base_entity* local, c_base_entity* target)
{
	if (!local->is_valid_player())
		return false;

	trace_t Trace;

	Vector src = local->get_vec_eyepos(), dst2 = target->get_bone(8);

	Ray_t ray;

	ray.Init(src, dst2);

	ITraceFilter traceFilter;

	traceFilter.pSkip = (void*)local;

	g_weebware.g_engine_trace->TraceRay(ray, MASK_SHOT, &traceFilter, &Trace);

	if (Trace.m_pEnt == target)
		return true;

	if (Trace.fraction == 1.0f)
		return true;

	return false;
}

