#pragma once

#include "c_rec.h"

class client_class
{
public:
	void* m_pCreateFn;
	void* m_pCreateEventFn;
	char* m_networkedname;
	recv_table* m_precvtable;
	client_class* m_pNext;
	int m_ClassID;
};

class i_base_client
{
public:

	client_class* get_all_classes()
	{
		return getvfunc<client_class* (__thiscall*)(PVOID)>(this, 8)(this);
	}

};
