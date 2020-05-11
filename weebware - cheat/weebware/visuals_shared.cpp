#include "Header.h"
#include "visuals_shared.h"

Vector center_hitbox(c_base_entity* ent, int id)
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


s_boundaries calc_boundaries(c_base_entity* Entity)
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

	if (g_maths.CRWorldToScreen(head, ScreenTop) && g_maths.CRWorldToScreen(origin, ScreenBottom))
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
