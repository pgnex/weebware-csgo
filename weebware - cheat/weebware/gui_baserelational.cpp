#include "graphics.h"

baserelational_t::~baserelational_t()
{
	/* Destroy All Children. */
	for (auto i = m_children.begin(); i < m_children.end(); i++)
	{
		delete *i;
	}
}

void baserelational_t::render()
{
	for (auto i = m_children.rbegin(); i < m_children.rend(); i++)
	{
		(*i)->render();
	}
}

void baserelational_t::update(deltatime_t delta)
{
	for (auto i = m_children.begin(); i < m_children.end(); i++)
	{
		(*i)->update(delta);
	}
}

bool baserelational_t::input(input_type type, input_variable variable)
{
	for (auto i = m_children.begin(); i < m_children.end(); i++)
	{
		if ((*i)->input(type, variable) == inpm_processed)
		{
			return inpm_processed;
		}
	}

	return inpm_continue;
}

void baserelational_t::addchild(basegraphical_t * newchild)
{
	m_children.push_back(newchild);
}
