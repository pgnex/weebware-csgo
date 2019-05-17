#pragma once

#ifndef DME
#define DME

#include "Header.h"

class c_dme
{
public:

	void draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld);
	bool is_visible(c_base_entity* target);
	void glow();

	enum custom_mats : int
	{
		plain = 1,
		platinum,
		glass,
		crystal,
		gold,
		dark_chrome,
		glow_rim3d,
		wildfire_gold,
		crystal_blue,
		velvet,
		darude,
		max
	};

	// Returns a material based on type
	imaterial* borrow_mat(c_dme::custom_mats type);

private:

protected:

};
extern c_dme g_dme;
#endif