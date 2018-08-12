#pragma once

#ifndef DME
#define DME

#include "Header.h"

class c_dme
{
public:

	void draw_model_execute(void* thisptr, int edx, c_unknownmat_class* ctx, const c_unknownmat_class& state, const modelrenderinfo_t& pInfo, matrix3x4* pCustomBoneToWorld);

	enum custom_mats : int
	{
		platinum = 1,
		glass,
		crystal,
		gold,
		dark_chrome,
		plastic,
		glow,
		chams,
		chams_wire,
		chams_flat,
		chams_gloss,
		glow_rim3d,
		max
	};

	// Returns a material based on type
	imaterial* create_mat(c_dme::custom_mats type);

private:

protected:

};
extern c_dme g_dme;
#endif