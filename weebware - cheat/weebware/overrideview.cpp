#include "Header.h"

void overrideview::thirdperson() {

	if (!g_weebware.g_engine->is_connected() || !g_weebware.g_engine->is_in_game())
		return;

	// get local
	c_base_entity* local = g_weebware.g_entlist->getcliententity(g_weebware.g_engine->get_local());
	if (!local)
		return;


	if (local->m_iHealth() > 0) {
		float range = g_weebwarecfg.thirdperson_distance;

		// we're already in thirdperson. nothing to do
		if (!g_weebware.g_input->m_fCameraInThirdPerson)
			g_weebware.g_input->m_fCameraInThirdPerson = true;

		Vector view_angles;
		g_weebware.g_engine->get_view_angles(view_angles);
		Vector forward = Vector(0, 0, 0);
		g_maths.qangle_vector(view_angles, forward);
		view_angles.z = 0.f;
		Vector eye_pos = local->get_vec_eyepos();

		
		trace_t trace;
		Ray_t ray;
		ITraceFilter filter;
		filter.pSkip = local;
		ray.Init(eye_pos, eye_pos - forward * range);
		g_weebware.g_engine_trace->TraceRay(ray, MASK_SHOT, &filter, &trace);

		if (trace.fraction < 1.f)
			range *= trace.fraction;


		view_angles.z = range;
		g_weebware.g_input->m_fCameraInThirdPerson = true;
		g_weebware.g_input->m_vecCameraOffset = view_angles;
	
	}
	else {
		g_weebware.g_input->m_fCameraInThirdPerson = false;
		g_weebware.g_input->m_vecCameraOffset.z = 0;
	}
}
