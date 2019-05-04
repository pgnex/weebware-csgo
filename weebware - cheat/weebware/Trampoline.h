#pragma once

/*
	The constructor autonatically hooks the function and unhooks on decontrstuction of the object
	by unloading this module.
*/
class c_trampolines
{
public:
	c_trampolines() {};

	c_trampolines(LPVOID function_address, LPVOID proxy_address, LPVOID original);

	void unhook_func();

private:

	LPVOID m_function_addr;
	LPVOID m_proxy_addr;
	LPVOID m_original;
	DWORD m_length = 0;

	void hook_func();
};

