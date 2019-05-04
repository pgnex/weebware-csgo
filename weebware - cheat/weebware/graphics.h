#pragma once
#include <vector>
#include "vector.h"

#include "gui_definitions.h"
#include "gui_basepositioned.h"
#include "gui_basegraphical.h"
#include "gui_baserelational.h"
#include "gui_baserenderable.h"

#include "gui_base.h"


struct userinterface_t
{
	bool Setup(std::string m_GuiFile);
	void Run();

	guiorigin_t m_Origin;
};