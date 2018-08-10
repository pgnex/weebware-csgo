#pragma once

typedef void(*hookfun_t)(PCONTEXT pointers);
class Hook
{
private:
	static std::map<DWORD, hookfun_t> hooks;
public:
	static void init();
	static void addHook(DWORD address, hookfun_t hookFunction);
	static void removeHook(DWORD address);
	static long __stdcall exceptionFilter(PEXCEPTION_POINTERS info);
};
