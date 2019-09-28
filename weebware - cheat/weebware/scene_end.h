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
		disabled,
		plain,
		glow_cham = 2,
		flat,
		glass,
		crystal,
		gold,
		crystal_blue,
		max
	};

	// Returns a material based on type
	imaterial* borrow_mat(c_sceneend::custom_mats type);
	imaterial* generate_material(bool ignore, bool lit, bool wire_frame);

private:

protected:

};
extern c_sceneend g_sceneend;