#pragma once

#include "Header.h"

class c_sceneend
{
public:
	void scene_end();
	bool is_visible(c_base_entity* target);
	void glow();
	void chams();

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
	imaterial* borrow_mat(c_sceneend::custom_mats type);

private:

protected:

};
extern c_sceneend g_sceneend;