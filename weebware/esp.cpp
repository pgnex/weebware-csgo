#include "esp.h"
#include "drawing.h"
#include "Legit.h"
#include "paint_traverse.h"

c_esp g_esp;

bool has_esp_init = false;

void c_esp::esp_main()
{
	water_mark();

#if 0
	render_dark_overlay();
#endif

	if (g_weebwarecfg.vis_cfg.enable_visuals == 0)
	{
		return;
	}

	if (g_weebwarecfg.vis_cfg.enable_visuals == 2)
	{
		if (!GetAsyncKeyState(g_weebwarecfg.vis_cfg.enable_visuals_key))
		{
			return;
		}
	}

	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
	{
		local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local()); // getting localplayer

		if (local)
		{
			if (local->is_valid_player())
			{
				draw_inaccuracy_circle();
				display_backtrack();

				if (g_weebwarecfg.vis_cfg.visuals_backtrack_dots) {

					for (size_t i = 0; i < g_accuracy.accuracy_records.size(); i++)
					{
						Vector w2s;

						if (g_maths.world_to_screen(g_accuracy.accuracy_records[i].m_head, w2s))
						{
							g_accuracy.accuracy_records[i].m_w2s_head = w2s;
							g_accuracy.accuracy_records[i].m_has_w2s = true;
						}
					}
#if 1

					for (int i = 0; i < g_accuracy.m_best_record.bonecount; i++)
					{
						if (g_maths.world_to_screen(g_accuracy.m_best_record.parent[i], m_skeleton_backtrack.w2s_parent[i])) {
							m_skeleton_backtrack.has_w2s_parent[i] = true;
						}

						if (g_maths.world_to_screen(g_accuracy.m_best_record.child[i], m_skeleton_backtrack.w2s_child[i])) {
							m_skeleton_backtrack.has_w2s_child[i] = true;
						}

					}

#endif
				}

			}

			for (int i = 1; i <= g_weebware.g_entlist->getmaxentities(); i++)
			{
				c_base_entity* ent = g_weebware.g_entlist->getcliententity(i);

				if (!ent || ent == nullptr)
					continue;

				// We need to find if the planted bomb exists...
				if (g_weebwarecfg.vis_cfg.visuals_bomb_timer && strstr(ent->get_client_class()->m_networkedname, "CPlantedC4"))
				{
					float remaining = reinterpret_cast<c_bomb*>(ent)->get_blow_time() - g_weebware.g_global_vars->curtime;

					// So c4 is still going!
					if (reinterpret_cast<c_bomb*>(ent)->is_ticking() && remaining > 0.f) {

						int offset_y = 5;

						if (g_weebwarecfg.vis_cfg.visuals_watermark) {
							offset_y += 20.f;
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

						g_paint_traverse.draw_string(g_weebware.tahoma_font,5, offset_y, predicted_damage < local->m_iHealth() ? c_color(0, 255, 0, 255) : c_color(255, 0, 0, 255), 0, timer_s);
			
						// Okay now lets check if we have defuse on the bomb..
						// m_flDefuseCountDown // interesting netvar let see what it returns...

					}

				}


#pragma region players
				if (!ent || ent->m_iHealth() <= 0 || ent->get_client_class()->m_ClassID != 35) {

					continue;
				}

				if (!g_weebwarecfg.vis_cfg.visuals_dormant_esp) {
					if (ent->is_dormant()) {
						continue;
					}
				}

				if (!g_weebwarecfg.vis_cfg.visuals_teammates && ent->m_iTeamNum() == local->m_iTeamNum()) {
					continue;
				}

				if (ent == local) {

					continue;
				}

				if (g_weebwarecfg.vis_cfg.visuals_bspotted) {
					*ent->b_spotted() = true;
				}

				if (g_weebwarecfg.vis_cfg.visuals_visible_only && !is_visible(local, ent)) {
					continue;
				}

				w2s_player[i].boundary = calc_boundaries(ent);

				if (ent->is_dormant())
					w2s_player[i].boundary.dormant = true;

				render_box(w2s_player[i].boundary, ent->m_iTeamNum() == local->m_iTeamNum());

				render_health(w2s_player[i].boundary, ent, ent->m_iTeamNum() == local->m_iTeamNum());
#pragma endregion

			}

		}
	}
}

void c_esp::esp_reset()
{
	if (!has_esp_init)
	{
		return;
	}

	g_draw.OnLostDevice();

	if (g_draw.Font())
	{
		g_draw.FontReset();
	}

	g_draw.Reset();
}

void c_esp::calc_w2svalues()
{
	if (g_weebware.g_engine->is_connected() && g_weebware.g_engine->is_in_game())
	{
		local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local()); // getting localplayer

		if (local)
		{

			for (int i = 1; i <= 99; i++)
			{
				c_base_entity* ent = g_weebware.g_entlist->getcliententity(i);

				if (!ent || ent->m_iHealth() <= 0 || ent->get_client_class()->m_ClassID != 35) {
					continue;
				}

				if (!g_weebwarecfg.vis_cfg.visuals_dormant_esp) {
					if (ent->is_dormant()) {
						continue;
					}
				}

				if (!g_weebwarecfg.vis_cfg.visuals_teammates && ent->m_iTeamNum() == local->m_iTeamNum())
				{
					continue;
				}

				if (i == local->index)
				{
					continue;
				}

				if (g_weebwarecfg.vis_cfg.visuals_visible_only && !is_visible(local, ent)) {
					continue;
				}

			

			}
		}


	}
}

void c_esp::water_mark()
{
	if (g_weebwarecfg.vis_cfg.visuals_watermark) {
		/* Testing purposes. */

		c_color watermark(g_weebwarecfg.sets_cfg.water_mark);

		g_paint_traverse.draw_water_mark();
	}
}
// Returns the center of a hitbox
// Could have globalised legitbots one and used it
Vector c_esp::center_hitbox(c_base_entity* ent, int id)
{
	if (!ent)
	{
		return Vector(0, 0, 0);
	}

	matrix3x4 matrix[128];

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

void c_esp::render_box(s_boundaries bounds, bool is_team)
{
	if (!g_weebwarecfg.vis_cfg.visuals_bounding_box)
	{
		return;
	}
	c_color col = is_team ? c_color(g_weebwarecfg.sets_cfg.visuals_bounding_team) : c_color(g_weebwarecfg.sets_cfg.visuals_bounding);

	// Box
	g_weebware.g_surface->drawsetcolor(0, 0, 0, 60);

	g_weebware.g_surface->drawoutlinedrect(bounds.x - 1, bounds.y - 1, bounds.w + 2 + bounds.x - 1, bounds.h + 2 + bounds.y - 1);

	if (g_weebwarecfg.vis_cfg.visuals_dormant_esp) {

		if (bounds.dormant) {
			
			c_color col = is_team ? c_color(g_weebwarecfg.sets_cfg.visuals_dormant_col_team) : c_color(g_weebwarecfg.sets_cfg.visuals_dormant_col);
		}

	}
	g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);

	g_weebware.g_surface->drawoutlinedrect(bounds.x, bounds.y, bounds.w + bounds.x, bounds.h + bounds.y);

#if 0
	// Top
	g_draw.Line(bounds.x + (bounds.w / 3), bounds.y, bounds.x + (bounds.w / 3 * 2), bounds.y, 3, true, D3DCOLOR_RGBA(255, 0, 0, 255));
	// Left
	g_draw.Line(bounds.x, bounds.y + (bounds.h / 3), bounds.x, bounds.y + (bounds.h / 3 * 2), 3, true, D3DCOLOR_RGBA(255, 0, 0, 255));
	// Right
	g_draw.Line(bounds.x + bounds.w, bounds.y + (bounds.h / 3), bounds.x + bounds.w, bounds.y + (bounds.h / 3 * 2), 3, true, D3DCOLOR_RGBA(255, 0, 0, 255));
	// Bottom
	g_draw.Line(bounds.x + (bounds.w / 3), bounds.y + bounds.h, bounds.x + (bounds.w / 3 * 2), bounds.y + bounds.h, 3, true, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif
}

void c_esp::render_health(s_boundaries bounds, c_base_entity* ent, bool is_team)
{
	if (!g_weebwarecfg.vis_cfg.visuals_health_bars)
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

	if (g_weebwarecfg.vis_cfg.visuals_dormant_esp) {
		if (bounds.dormant) {
			col = is_team ? c_color(g_weebwarecfg.sets_cfg.visuals_dormant_col_team) : c_color(g_weebwarecfg.sets_cfg.visuals_dormant_col);
		}
	}

	g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);

	g_weebware.g_surface->drawline(bounds.x - 5, bounds.y + length, bounds.x - 5, bounds.y + bounds.h);

	if (ent->m_iHealth() < 100 && ent->m_iHealth() > 0) {
		
		g_paint_traverse.draw_string(g_weebware.tahoma_font, bounds.x - 5, bounds.y + length,c_color(255, 255, 255, 255), 0, (char*)std::to_string(ent->m_iHealth()).c_str());
	
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

void c_esp::draw_inaccuracy_circle()
{
	if (!g_weebwarecfg.vis_cfg.visuals_inacc_circle)
		return;

	int x, y;
	g_weebware.g_engine->get_screen_dimensions(x, y);
	c_color col = c_color(g_weebwarecfg.sets_cfg.visuals_innacc_circle);

	g_weebware.g_surface->drawcoloredcircle(x / 2, y / 2, local->m_pActiveWeapon()->Get_Innacuracy() * 200, col.r, col.g, col.b, col.a);
}

#if 0
void render_dark_overlay()
{
	if (g_entry.menu_opened)
	{
		int s, h;
		g_entry.g_engine->get_screen_dimensions(s, h);

		c_color col = c_color(g_weebwarecfg.sets_cfg.visuals_bounding_team);

		g_draw.BoxFilled(0, 0, s, h, D3DCOLOR_RGBA(0, 0, 0, 150));
	}
}
#endif

void c_esp::display_backtrack()
{
	if (!g_weebwarecfg.vis_cfg.visuals_backtrack_dots)
		return;

#if 0
	for (auto record : g_accuracy.accuracy_records)
	{
		if (record.m_has_w2s)
			g_draw.Line(record.m_w2s_head.x, record.m_w2s_head.y, record.m_w2s_head.x + 1, record.m_w2s_head.y, 2, false, record.m_best_record ? D3DCOLOR_RGBA(0, 255, 0, 255) : D3DCOLOR_RGBA(255, 0, 0, 255));
	}
#endif

#if 1
	if (!g_accuracy.m_best_record.player->is_valid_player())
		return;

	c_color col = c_color(g_weebwarecfg.sets_cfg.visuals_backtrack_col);

	int max_width, max_height;
	g_weebware.g_engine->get_screen_dimensions(max_width, max_height);

	// Check if we have correct dimensions...

	for (int i = 0; i < g_accuracy.m_best_record.bonecount; i++) {

		if (m_skeleton_backtrack.w2s_parent[i].x >= max_width || m_skeleton_backtrack.w2s_parent[i].x <= 0
			|| m_skeleton_backtrack.w2s_parent[i].y >= max_height || m_skeleton_backtrack.w2s_parent[i].y <= 0
			|| m_skeleton_backtrack.w2s_child[i].x >= max_width || m_skeleton_backtrack.w2s_child[i].x <= 0
			|| m_skeleton_backtrack.w2s_child[i].y >= max_height || m_skeleton_backtrack.w2s_child[i].y <= 0
			|| !m_skeleton_backtrack.has_w2s_child[i] || !m_skeleton_backtrack.has_w2s_parent[i])
			continue;

		g_weebware.g_surface->drawsetcolor(col.r, col.g, col.b, col.a);

		g_weebware.g_surface->drawline(m_skeleton_backtrack.w2s_parent[i].x, m_skeleton_backtrack.w2s_parent[i].y, m_skeleton_backtrack.w2s_child[i].x, m_skeleton_backtrack.w2s_child[i].y);
	}
#endif
	}

bool c_esp::is_visible(c_base_entity* local, c_base_entity* target)
{
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

