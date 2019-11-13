#include "Ragebot.h"

namespace ragebot
{
	bool enabled;

	bool auto_wall::asm_trace_to_exit( Vector& end, trace_t& tr, float x, float y, float z, float x2, float y2, float z2, trace_t* trace )
	{
		static auto trace_to_exit = reinterpret_cast<trace_to_exit_fn>(g_weebware.pattern_scan(
			"client_panorama.dll",
			"55 8B EC 83 EC 30 F3 0F 10 75" ));

		if ( !trace_to_exit )
			return false;

		_asm {
			push trace
			push z2
			push y2
			push x2
			push z
			push y
			push x
			mov edx, tr
			mov ecx, end
			call trace_to_exit
			add esp, 0x1C
		}
	}

	bool auto_wall::handle_bullet_pen( c_weapon_info* wpn_data, fire_bullet_data& data )
	{
		surfacedata_t* enter_surface_data = reinterpret_cast<surfacedata_t*>(g_weebware.g_surface_props->GetSurfaceData( data.enter_trace.surface.surfaceProps ));
		int enter_material = enter_surface_data->game.material;
		float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;
		data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
		data.current_damage *= pow( (wpn_data->range_modifier), (data.trace_length * 0.002) );
		if ( (data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f) )
			data.penetrate_count = 0;
		if ( data.penetrate_count <= 0 )
			return false;
		Vector dummy;
		trace_t trace_exit;
		if ( !this->asm_trace_to_exit(
			dummy,
			data.enter_trace,
			data.enter_trace.endpos.x,
			data.enter_trace.endpos.y,
			data.enter_trace.endpos.z,
			data.direction.x,
			data.direction.y,
			data.direction.z,
			&trace_exit ) )
			return false;
		surfacedata_t* exit_surface_data = reinterpret_cast<surfacedata_t*>(g_weebware.g_surface_props->GetSurfaceData( trace_exit.surface.surfaceProps ));
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
		float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf( 0.0f, (3.0f / wpn_data->m_Penetration) * 1.25f );
		float thickness = (trace_exit.endpos - data.enter_trace.endpos).length( );
		thickness *= thickness;
		thickness *= v34;
		thickness /= 24.0f;
		float lost_damage = fmaxf( 0.0f, v35 + thickness );
		if ( lost_damage > data.current_damage )
			return false;
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
		filter.pSkip = ignore;
		g_weebware.g_engine_trace->TraceRay( ray, mask, &filter, ptr );
	}

	void auto_wall::clip_trace_to_players( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr ) {

		static auto clip_trace_fn = g_weebware.pattern_scan(
			"client_panorama.dll",
			"53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10" );

		if ( !clip_trace_fn )
			return;

		_asm {
			MOV		EAX, filter
			LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	clip_trace_fn
			ADD		ESP, 0xC
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
		data.current_damage = (float)weaponData->m_WeaponDamage;
		while ( (data.penetrate_count > 0) && (data.current_damage >= 1.0f) ) {
			data.trace_length_remaining = weaponData->range - data.trace_length;
			Vector end = data.src + data.direction * data.trace_length_remaining;

			// SOMETHING IS WRONG HERE I THINK?
			this->trace_line( data.src, end, MASK_SHOT | CONTENTS_GRATE, local, &data.enter_trace );
			U::ClipTraceToPlayers( data.src, end + data.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, &data.filter, &data.enter_trace );

			if ( data.enter_trace.fraction == 1.0f )
				break;
			if ( (data.enter_trace.hitgroup <= 7) && (data.enter_trace.hitgroup > 0) ) {
				data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
				data.current_damage *= pow( weaponData->range_modifier, data.trace_length * 0.002 );
				ScaleDamage( data.enter_trace.hitgroup, data.enter_trace.m_pEnt, weaponData->m_ArmorRatio, data.current_damage );
				return true;
			}
			if ( !this->handle_bullet_pen( weaponData, data ) )
				break;
		}
		return false;
	}

	c_base_entity* get_ideal_target( )
	{
		return {};
	}

	void main( )
	{
		if ( !enabled ) {
			return;
		}



	}



}