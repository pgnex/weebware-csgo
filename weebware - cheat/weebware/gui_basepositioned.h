#pragma once
/*
	PURPOSE:
		Holds Position And Transformation Information
	Needs Work
*/
struct basepositioned_t
{
	basepositioned_t(Vector position) : m_position(position) {};
	Vector m_position;
};