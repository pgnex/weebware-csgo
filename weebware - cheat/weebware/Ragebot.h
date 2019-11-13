#pragma once
#include "Header.h"

namespace ragebot
{
	enum class target_method
	{
		none = 0,
		closest = 1,
		lowest = 2
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

	struct surfacephysicsparams_t
	{
		float    friction;
		float    elasticity;
		float    density;
		float    thickness;
		float    dampening;
	};

	struct surfaceaudioparams_t
	{
		float    reflectivity;             // like elasticity, but how much sound should be reflected by this surface
		float    hardnessFactor;           // like elasticity, but only affects impact sound choices
		float    roughnessFactor;          // like friction, but only affects scrape sound choices   
		float    roughThreshold;           // surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
		float    hardThreshold;            // surface hardness > this causes "hard" impacts, < this causes "soft" impacts
		float    hardVelocityThreshold;    // collision velocity > this causes "hard" impacts, < this causes "soft" impacts   
		float    highPitchOcclusion;       //a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
		float    midPitchOcclusion;
		float    lowPitchOcclusion;
	};

	struct surfacesoundnames_t
	{
		unsigned short    walkStepLeft;
		unsigned short    walkStepRight;
		unsigned short	  runStepLeft;
		unsigned short	  runStepRight;
		unsigned short    impactSoft;
		unsigned short    impactHard;
		unsigned short    scrapeSmooth;
		unsigned short    scrapeRough;
		unsigned short    bulletImpact;
		unsigned short    rolling;
		unsigned short    breakSound;
		unsigned short    strainSound;
	};

	struct surfacegameprops_t
	{
	public:
		float maxSpeedFactor;
		float jumpFactor;
		float flPenetrationModifier;
		float flDamageModifier;
		unsigned short material;
		byte climbable;
		char pad00[0x4];

	};

	struct surfacedata_t
	{
		surfacephysicsparams_t    physics;
		surfaceaudioparams_t    audio;
		surfacesoundnames_t        sounds;
		surfacegameprops_t        game;
	};

	class auto_wall
	{
	public:
		//	Call to trace to exit function using asm
		bool asm_trace_to_exit( Vector& end, trace_t& tr, float x, float y, float z, float x2, float y2, float z2, trace_t* trace );

		// Rebuilds handlebulletpentration function from source sdk
		bool handle_bullet_pen( c_weapon_info* wpn_data, fire_bullet_data& data );

		void trace_line( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, c_base_entity* ignore, trace_t* ptr );

		void clip_trace_to_players( Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr );

		bool sim_bullet_fire( fire_bullet_data& data, c_base_entity* local );
	};
	class target_info
	{
	public:
		std::vector<Vector> hitboxes;
	};

	void main( );
}