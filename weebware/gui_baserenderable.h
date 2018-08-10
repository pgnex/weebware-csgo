#pragma once
/*
	PURPOSE:
		Holds Themeing Information for all Objects being Rendered.
*/
struct baserenderable_t
{
public:
	baserenderable_t(theme_t* theme) :
		m_theme(theme) {};
	
	virtual ~baserenderable_t();

protected:
	theme_t* m_theme;
};