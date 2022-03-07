#include "Header.h"
#include "shared_funcs.h"

bool is_visible(c_base_entity* local, c_base_entity* target, int bone)
{

	if (!local)
		local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());

	trace_t Trace;

	Vector src = local->get_vec_eyepos(), dst2 = target->get_bone(bone); // 8 is head. 

	ray_t ray(src, dst2);

	trace_filter traceFilter(local);

	g_weebware.g_engine_trace->trace_ray(ray, MASK_SHOT, &traceFilter, &Trace);

	if (!Trace.entity->is_valid_player())
		return false;

	if ( Trace.entity == target || Trace.flFraction == 1.0f)
		return true;

	return false;
}

int get_config_index()
{
	auto local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
	auto weapon = local->m_pActiveWeapon();

	if (!local) {
		return 0;
	}

	if (!weapon) {
		return 0;
	}

	if (weapon->is_pistol())
	{
		return 0;
	}
	else if (weapon->is_rifle())
	{
		return 1;
	}
	else if (weapon->is_smg())
	{
		return 2;
	}
	else if (weapon->is_shotgun())
	{
		return 3;
	}
	else if (weapon->is_heavy())
	{
		return 4;
	}
	else if (weapon->is_autosniper())
	{
		return 5;
	}
	else if (weapon->is_awp())
	{
		return 6;
	}
	else if (weapon->is_scout())
	{
		return 7;
	}

	// This shouldn't ever happen.
	return 0;
}


long long get_epoch_ms() {
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}


long get_epoch_s() {
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

float random_float(float flMinVal, float flMaxVal)
{
	typedef float(__cdecl* RandomFloatFn)(float, float);
	static RandomFloatFn randomFloat = (RandomFloatFn)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat");
	return randomFloat(flMinVal, flMaxVal);
}

void random_seed(UINT Seed)
{
	typedef void(*RandomSeed_t)(UINT);
	static RandomSeed_t m_RandomSeed = (RandomSeed_t)GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomSeed");
	m_RandomSeed(Seed);
}

Vector get_spread(c_basecombat_weapon* weapon, int seed)
{
	random_seed((seed & 0xFF) + 1);

	constexpr float pi = PI;
	constexpr float pi2 = pi * 2;

	float a = random_float(0, pi2);
	float flRandomInaccuracy = random_float(0, weapon->Get_Innacuracy());
	float c = random_float(0, pi2);

	float flRandomSpread = random_float(0, weapon->GetSpread());

	float x = (cos(a) * flRandomSpread) + (cos(c) * flRandomInaccuracy);
	float y = (sin(a) * flRandomSpread) + (sin(c) * flRandomInaccuracy);
	return Vector(x, y, 0);
}