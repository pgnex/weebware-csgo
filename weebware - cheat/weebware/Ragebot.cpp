#include "Header.h"
#include "Ragebot.h"

namespace ragebot
{
	bool enabled = true;
	bool friendly_fire = false;
	int target_method = static_cast<int>(target_method::lowest_hp);
	auto_wall g_autowall;
//	float min_dmg = 1.f;

	bool auto_wall::asm_trace_to_exit( Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace )
	{
		static DWORD trace_exit_address = g_weebware.pattern_scan( "client_panorama.dll", "55 8B EC 83 EC 30 F3 0F 10 75" );

		if ( !trace_exit_address )
			return false;

		float start_y = start.y, start_z = start.z, start_x = start.x, dir_y = vEnd.y, dir_x = vEnd.x, dir_z = vEnd.z;

		_asm
		{
			push trace
			push dir_z
			push dir_y
			push dir_x
			push start_z
			push start_y
			push start_x
			mov edx, tr
			mov ecx, end
			call trace_exit_address
			add esp, 0x1C
		}
	}

	bool auto_wall::handle_bullet_pen( c_weapon_info* wpn_data, fire_bullet_data& data )
	{
		surfacedata_t* enter_surface_data = g_weebware.g_surface_props->GetSurfaceData( data.enter_trace.surface.surfaceProps );
		int enter_material = enter_surface_data->game.material;
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
		data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
		data.current_damage *= pow( (wpn_data->flRangeModifier), (data.trace_length * 0.002) );
		if ( (data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f) ) {
		//	printf( "trace out of range\n" );
			data.penetrate_count = 0;
		}
		if ( data.penetrate_count <= 0 ) {
		//	printf( "pen count ended\n" );
			return false;
		}
		Vector dummy;
		trace_t trace_exit;
		//printf( "about to trace\n" );
		if ( !this->asm_trace_to_exit(
			dummy,
			data.enter_trace,
			data.enter_trace.endpos,
			data.direction,
			&trace_exit ) )
			return false;
		surfacedata_t* exit_surface_data = g_weebware.g_surface_props->GetSurfaceData( trace_exit.surface.surfaceProps );
		int exit_material = exit_surface_data->game.material;
		float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
		float final_damage_modifier = 0.16f;
		float combined_penetration_modifier = 0.0f;
		if ( ((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71) )
		{
			combined_penetration_modifier = 3.0f;
			final_damage_modifier = 0.05f;
		}
		else
		{
			combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
		}
		if ( enter_material == exit_material )
		{
			if ( exit_material == 87 || exit_material == 85 )
				combined_penetration_modifier = 3.0f;
			else if ( exit_material == 76 )
				combined_penetration_modifier = 2.0f;
		}
		float v34 = fmaxf( 0.f, 1.0f / combined_penetration_modifier );
		float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf( 0.0f, (3.0f / wpn_data->flPenetration) * 1.25f );

		//printf( "cur damage: %.2f\n", data.current_damage );
		float thickness = (trace_exit.endpos - data.enter_trace.endpos).length( );
		thickness *= thickness;
		thickness *= v34;
		thickness /= 24.0f;

		float lost_damage = fmaxf( 0.0f, v35 + thickness );

		if ( lost_damage > data.current_damage ) {
		//	printf( "damage lost is too great \n" );
			return false;
		}
		if ( lost_damage >= 0.0f )
			data.current_damage -= lost_damage;
		if ( data.current_damage < 1.0f )
			return false;
		data.src = trace_exit.endpos;
		data.penetrate_count--;
		return true;
	}

	void auto_wall::trace_line( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, c_base_entity* ignore, trace_t* ptr ) {
		Ray_t ray;
		ray.Init( vecAbsStart, vecAbsEnd );
		ITraceFilter filter;
		filter.pSkip = reinterpret_cast<void*>(ignore);
		g_weebware.g_engine_trace->TraceRay( ray, mask, &filter, ptr );
	}

	Vector Normalize( Vector to_norm )
	{
		Vector res = to_norm;
		float l = res.length( );
		if ( l != 0.0f ) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	float dst_to_ray( const Vector& pos, const Vector& rayStart, const Vector& rayEnd, float* along = NULL, Vector* pointOnRay = NULL )
	{
		Vector to = pos - rayStart;
		Vector dir = Normalize( rayEnd - rayStart );
		float length = dir.length( );

		float rangeAlong = g_maths.dot_product( dir, to );
		if ( along )
		{
			*along = rangeAlong;
		}

		float range;

		if ( rangeAlong < 0.0f )
		{
			// off start point
			range = -(pos - rayStart).length( );

			if ( pointOnRay )
			{
				*pointOnRay = rayStart;
			}
		}
		else if ( rangeAlong > length )
		{
			// off end point
			range = -(pos - rayEnd).length( );

			if ( pointOnRay )
			{
				*pointOnRay = rayEnd;
			}
		}
		else // within ray bounds
		{
			Vector onRay = rayStart + (dir.operator*( rangeAlong ));
			range = (pos - onRay).length( );

			if ( pointOnRay )
			{
				*pointOnRay = onRay;
			}
		}

		return range;
	}

	void auto_stop(c_usercmd* cmd)
	{
		cmd->forwardmove = 0;
		cmd->sidemove = 0;
	}


	bool raytrace_hc(Vector viewAngles, float chance, c_base_entity* target, float dst, c_base_entity* local)
	{
		if (chance <= 0.f) return true;
		int hitCount = 0;
		Vector vecDirShooting, vecRight, vecUp;
		g_maths.qangle_vector(viewAngles, vecDirShooting, vecRight, vecUp);
		c_trace_customfilter traceFilter;

		for (int seed = 0; seed < 255; seed++)
		{
			Vector spread = get_spread(local->m_pActiveWeapon(), seed);

			Vector vecDir(vecDirShooting.x + spread.x * vecRight.x + spread.y * vecUp.x,
				vecDirShooting.y + spread.x * vecRight.y + spread.y * vecUp.y,
				vecDirShooting.z + spread.x * vecRight.z + spread.y * vecUp.z);

			Vector vecEnd = local->get_vec_eyepos() + (vecDir * dst);
			trace_t trace;
			Ray_t ray;
			traceFilter.pTarget = (void*)target;
			ray.Init(local->get_vec_eyepos(), vecEnd);
			g_weebware.g_engine_trace->TraceRay(ray, 0x4600400B, &traceFilter, &trace);

			if (trace.fraction == 1.0f)
				continue;

			if (trace.m_pEnt == target)
				hitCount++;
		}

		if (hitCount >= (255 * (chance / 100.0f)))
			return true;

		return false;
	}

	bool next_attack_queued(c_base_entity* local)
	{
		if (local->m_pActiveWeapon()->m_flNextPrimaryAttack() > (float)g_weebware.g_global_vars->curtime)
			return false;
		return true;
	}


	void auto_wall::clip_trace_to_player( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr )
	{
		trace_t playerTrace;
		Ray_t ray;
		float smallestFraction = tr->fraction;
		const float maxRange = 60.0f;

		ray.Init( vecAbsStart, vecAbsEnd );

		for ( int k = 1; k <= g_weebware.g_engine->get_max_clients( ); k++ )
		{
			c_base_entity* player = g_weebware.g_entlist->getcliententity( k );

			if ( !player || !player->is_valid_player( ) )
				continue;

			if ( player->IsDormant( ) )
				continue;

			if ( filter && filter->ShouldHitEntity( player, mask ) == false )
				continue;

			Vector Max = *player->m_vecMaxs( ) + *player->m_vecOrigin( );
			Vector Min = *player->m_vecMins( ) + *player->m_vecOrigin( );
			Vector world_space_center = Max - Min;
			world_space_center /= 2;
			world_space_center += Min;

			float range = dst_to_ray( world_space_center, vecAbsStart, vecAbsEnd );
			if ( range < 0.0f || range > maxRange )
				continue;

			g_weebware.g_engine_trace->ClipRayToEntity( ray, mask | CONTENTS_HITBOX, reinterpret_cast<IHandleEntity*>(player), &playerTrace );
			if ( playerTrace.fraction < smallestFraction )
			{
				// we shortened the ray - save off the trace
				*tr = playerTrace;
				smallestFraction = playerTrace.fraction;
			}
		}
	}


	float auto_wall::hitgroup_dmg_ratio( int hit_group )
	{
		switch ( hit_group )
		{
		case 1: // head
		{
			return 4.0f;
		}
		case 2: // chest
		case 4: // left arm
		case 5: // right arm
		{
			return 1.0f;
		}
		case 3: // stomach
		{
			return 1.25f;
		}
		case 6: // left leg
		case 7: // right leg
		{
			return 0.75f;
		}
		}
		return 1.0f;
	}

	void auto_wall::scale_dmg( int hitgroup, c_base_entity* enemy, float weapon_armor_ratio, float& current_damage )
	{
		current_damage *= hitgroup_dmg_ratio( hitgroup );

		//if ( !friendly_fire && enemy->m_iTeamNum( ) == G::LocalPlayer->GetTeam( ) )
		//{
		//	current_damage = 0;
		//	return;
		//}

		int armor = enemy->m_ArmorValue( );
		int helmet = enemy->b_has_helmet( );

		if ( armor > 0 )
		{
			if ( hitgroup == 1 ) // if aiming at head
			{
				if ( helmet ) {
					current_damage *= (weapon_armor_ratio * .5f);
				}
			}
			else
			{
				current_damage *= (weapon_armor_ratio * .5f);
			}
		}
	}

	bool auto_wall::sim_bullet_fire( fire_bullet_data& data, c_base_entity* local )
	{
		data.penetrate_count = 4;
		data.trace_length = 0.0f;
		auto local_weapon = local->m_pActiveWeapon( );
		if ( !local_weapon )
			return false;
		c_weapon_info* weaponData = local_weapon->get_weapon_info( );
		if ( !weaponData )
			return false;
		data.current_damage = static_cast<float>(weaponData->iDamage);
		while ( (data.penetrate_count > 0) && (data.current_damage >= 1.0f) ) {
			data.trace_length_remaining = weaponData->flRange - data.trace_length;
			Vector end = data.src + data.direction * data.trace_length_remaining;

			//this->trace_line( data.src, end, MASK_SHOT | CONTENTS_GRATE, local, &data.enter_trace );
			// this->clip_trace_to_player( data.src, end + data.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, &data.filter, &data.enter_trace );

			// this->trace_line( data.src, end, MASK_SHOT | CONTENTS_GRATE, local, &data.enter_trace );
			// this->clip_trace_to_players( data.src, end + data.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, &data.filter, &data.enter_trace );

			this->trace_line( data.src, end + data.direction * 40.f, MASK_SHOT, local, &data.enter_trace );
		//	printf( "traced line \n" );
			if ( data.enter_trace.fraction == 1.0f ) {
			//	printf( "fraction 1 \n" );
				break;
			}
			if ( (data.enter_trace.hitgroup > 0) && (data.enter_trace.hitgroup <= 7) ) {
			//	printf( "hitgroup inbound\n" );
				data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
				data.current_damage *= pow( weaponData->flRangeModifier, data.trace_length * 0.002 );
				scale_dmg( data.enter_trace.hitgroup, data.enter_trace.m_pEnt, weaponData->flArmorRatio, data.current_damage );
				return true;
			}
			if ( !this->handle_bullet_pen( weaponData, data ) )
				break;
		}
		return false;
	}

	float auto_wall::get_dmg( c_base_entity* local, Vector& point ) {
		float damage = 0.f;
		Vector dst = point;
		fire_bullet_data data;
		data.src = local->get_vec_eyepos( );
		data.filter.pSkip = local;
		QAngle angles {};
		g_maths.vector_qangles( dst - data.src, angles );
		g_maths.qangle_vector( angles, data.direction );
		Vector dataNormalized;
		data.direction = Normalize( data.direction );
		if ( this->sim_bullet_fire( data, local ) )
			damage = data.current_damage;
		return damage;
	}

	Vector center_hitbox_r( c_base_entity* ent, int id )
	{
		if ( !ent ) {
			return {};
		}
		matrix3x4_t matrix[128];
		if ( !ent->setup_bones( matrix, 128, 0x00000100, g_weebware.g_global_vars->curtime ) ) {
			return {};
		}
		const model_t* mod = ent->get_model( );
		if ( !mod ) {
			return {};
		}
		studiohdr_t* hdr = g_weebware.g_model_info->getstudiomodel( mod );
		if ( !hdr ) {
			return {};
		}
		mstudiohitboxset_t* set = hdr->GetHitboxSet( 0 );
		if ( !set ) {
			return {};
		}
		mstudiobbox_t* hitbox = set->pHitbox( id );
		if ( !hitbox ) {
			return {};
		}
		Vector vMin, vMax, vCenter;
		g_maths.VectorTransform2( hitbox->bbmin, matrix[hitbox->bone], vMin );
		g_maths.VectorTransform2( hitbox->bbmax, matrix[hitbox->bone], vMax );
		vCenter = (vMin + vMax) * 0.5f;
		return vCenter;
	}

	std::vector<int> hitbox_simple =
	{
		csgohitboxid::head,
		csgohitboxid::neck,
		csgohitboxid::pelvis,
		csgohitboxid::stomach,
		csgohitboxid::chest
	};

	c_base_entity* get_ideal_target( c_base_entity* local )
	{
		int32_t smallest_dst = 99999;
		int32_t smallest_fov = 180;
		int32_t smallest_health = 101;

		c_base_entity* ideal_target = nullptr;

		for ( int i = 1; i <= g_weebware.g_engine->get_max_clients( ); i++ ) {

			c_base_entity* cur_entity = g_weebware.g_entlist->getcliententity( i );

			if ( !cur_entity->is_valid_player( ) )
				continue;

			if (cur_entity->m_bGunGameImmunity())
				continue;

			if ( !g_weebwarecfg.ragebot_target_team &&
				(cur_entity->m_iTeamNum( ) == local->m_iTeamNum( )) )
				continue;
			
			// ignore unhittable players
			if ( g_autowall.get_dmg( local, cur_entity->get_bone( 8 ) ) < 1.f ) // 8 is head.
				continue;


			// fov check if needed
			// really isn't needed?
			// its a rage cheat
			switch ( target_method ) {
				case static_cast<int>(target_method::distance) :
				{
					Vector delta = *local->m_vecOrigin( ) - *cur_entity->m_vecOrigin( );

					auto dst = delta.length( );

					if ( dst < smallest_dst ) {
						ideal_target = cur_entity;
						smallest_dst = dst;
					}

				}
				break;
				case static_cast<int>(target_method::crosshair) :
				{
					Vector center_head = center_hitbox_r(cur_entity, (int)csgohitboxid::head);
					QAngle angle_to_head;

					g_maths.vector_qangles(center_head - local->get_vec_eyepos(), angle_to_head);
					g_maths.normalize_angle(angle_to_head);

					QAngle view_angles = {};
					g_weebware.g_engine->get_view_angles(view_angles);
					float normal_fov = g_maths.get_fov(view_angles, angle_to_head);

					if (normal_fov < smallest_fov) {
						ideal_target = cur_entity;
						smallest_fov = normal_fov;
					}
				}
				break;
				case static_cast<int>(target_method::lowest_hp) :
				{

					// we need to make these functions so its easier to default :l

					int target_health = cur_entity->m_iHealth();

					if (target_health < smallest_health) {
						ideal_target = cur_entity;
						smallest_health = target_health;
					}
				}
				break;
				default: // method not selected
				{
					return {};
				}
			}
		}

		return ideal_target;
	}


	Vector hitscan_target( c_base_entity* target, c_base_entity* local, bool* can_fire )
	{
		if ( !target || !local )
			return {};

		auto best_dmg = 0.f;
		Vector best_pos = {};
		*can_fire = false;

		for ( auto hitbox : hitbox_simple ) {
			auto hitbox_pos = center_hitbox_r( target, hitbox );
			float dmg = 9999, min_dmg = 0;

			if (g_weebwarecfg.autowall_min_dmg > 0) {
				dmg = g_autowall.get_dmg(local, hitbox_pos);
				min_dmg = g_weebwarecfg.autowall_min_dmg;
			}


			if ( dmg > best_dmg && dmg > min_dmg) {
				*can_fire = true;
				best_pos = hitbox_pos;
				best_dmg = dmg;
			}
		}

		if ( !(*can_fire) )
			return {};

		return best_pos;
	}

	void main( c_base_entity* local, c_usercmd* cmd )
	{
		if ( !g_weebwarecfg.ragebot_enabled || !local ) {
			return;
		}

		auto target = get_ideal_target( local );

		if ( target ) {

			bool valid = false;
			auto best_pos = hitscan_target( target, local, &valid );

			if ( !valid ) {
				return;
			}

			QAngle target_ang = {};
			g_maths.vector_qangles( best_pos - local->get_vec_eyepos( ), target_ang );

			if (g_weebwarecfg.no_recoil)
				target_ang -= local->m_aimPunchAngle() * 2.f;

			cmd->viewangles = target_ang;

			if (!g_weebwarecfg.ragebot_silent_aim)
				g_weebware.g_engine->set_view_angles( cmd->viewangles );

			if (g_weebwarecfg.ragebot_autostop)
				if (!next_attack_queued(local))
					auto_stop(cmd);
			
			if (!g_weebwarecfg.autoshoot_enabled)
				return;

			QAngle view_angles = {};
			g_weebware.g_engine->get_view_angles(view_angles);

			if (raytrace_hc(cmd->viewangles, g_weebwarecfg.ragebot_hitchance, target, local->m_pActiveWeapon()->get_weapon_info()->flRange, local) && next_attack_queued(local)) {
				cmd->buttons |= in_attack;
			}
			else {
				cmd->buttons &= ~in_attack;
			}

		}
	}

}