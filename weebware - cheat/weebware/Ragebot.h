#pragma once
#include "Header.h"

namespace ragebot
{
	enum class target_method : int
	{
		none = 0,
		crosshair = 1,
		distance = 2,
		lowest_hp = 3
	};

	typedef bool( __fastcall* trace_to_exit_fn )(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);

	struct fire_bullet_data
	{
		Vector          src;
		trace_t         enter_trace;
		Vector          direction;
		ITraceFilter    filter;
		float           trace_length;
		float           trace_length_remaining;
		float           current_damage;
		int             penetrate_count;
	};


	class auto_wall
	{
	public:
		//	Call to trace to exit function using asm
		bool asm_trace_to_exit( Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace );

		// Rebuilds handlebulletpentration function from source sdk
		bool handle_bullet_pen( c_weapon_info* wpn_data, fire_bullet_data& data );

		void trace_line( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, c_base_entity* ignore, trace_t* ptr );

		// void clip_trace_to_players( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr );

		void clip_trace_to_player( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr );

		float hitgroup_dmg_ratio( int hit_group );

		void scale_dmg( int hitgroup, c_base_entity* enemy, float weapon_armor_ratio, float& current_damage );

		bool sim_bullet_fire( fire_bullet_data& data, c_base_entity* local );

		float get_dmg( c_base_entity* player, Vector& point );
	};

	extern auto_wall g_autowall;

	class target_info
	{
	public:
		std::vector<Vector> hitboxes;
	};

	void main( c_base_entity* local, c_usercmd* cmd );

}