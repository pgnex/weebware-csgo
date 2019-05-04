/*
	This trampoline hook class is a port from https://github.com/MalwareTech/BasicHook
*/

#include "Header.h"

#include "Disassembler\hde32.h"

#define NO_INLINE_ASM

c_trampolines::c_trampolines(LPVOID function_address, LPVOID proxy_address, LPVOID original) {

	// store the original addresses.
	m_function_addr = (LPVOID*)function_address;
	m_proxy_addr = (LPVOID*)proxy_address;
	m_original = (LPVOID*)original;
	m_length = 0;
	hook_func();
}

//We need to copy 5 bytes, but we can only do 2, 4, 8 atomically
//Pad buffer to 8 bytes then use lock cmpxchg8b instruction
void SafeMemcpyPadded(LPVOID destination, LPVOID source, DWORD size)
{
	BYTE SourceBuffer[8];

	if (size > 8)
		return;

	//Pad the source buffer with bytes from destination
	memcpy(SourceBuffer, destination, 8);
	memcpy(SourceBuffer, source, size);

#ifndef NO_INLINE_ASM
	__asm
	{
		lea esi, SourceBuffer;
		mov edi, destination;

		mov eax, [edi];
		mov edx, [edi + 4];
		mov ebx, [esi];
		mov ecx, [esi + 4];

		lock cmpxchg8b[edi];
	}
#else
	_InterlockedCompareExchange64((LONGLONG *)destination, *(LONGLONG *)SourceBuffer, *(LONGLONG *)destination);
#endif
}


void c_trampolines::hook_func() {

	DWORD tramp_length = 0, protection_;
	hde32s disam;
	BYTE jump_code[5] = { 0xE9, 0x00, 0x00, 0x00, 0x00 };

	//disassemble length of each instruction, until we have 5 or more bytes worth
	while (tramp_length < 5)
	{
		LPVOID InstPointer = (LPVOID)((DWORD)m_function_addr + tramp_length);
		tramp_length += hde32_disasm(InstPointer, &disam);
	}

	//Build the trampoline buffer
	memcpy(m_original, m_function_addr, tramp_length);
	*(DWORD *)(jump_code + 1) = ((DWORD)m_function_addr + tramp_length) - ((DWORD)m_original + tramp_length + 5);
	memcpy((LPVOID)((DWORD)m_original + tramp_length), jump_code, 5);

	//Make sure the function is writable
	if (!VirtualProtect(m_function_addr, tramp_length, PAGE_EXECUTE_READWRITE, &protection_))
		return;

	//Build and atomically write the hook
	*(DWORD *)(jump_code + 1) = (DWORD)m_proxy_addr - (DWORD)m_function_addr - 5;
	SafeMemcpyPadded(m_function_addr, jump_code, 5);

	//Restore the original page protection
	VirtualProtect(m_function_addr, tramp_length, protection_, &protection_);

	//Clear CPU instruction cache
	FlushInstructionCache(GetCurrentProcess(), m_function_addr, tramp_length);

	m_length = tramp_length;
}

void c_trampolines::unhook_func() {

	DWORD original_protection;

	VirtualProtect(m_function_addr, m_length, PAGE_EXECUTE_READWRITE, &original_protection);

	SafeMemcpyPadded(m_function_addr, m_original, m_length);

	VirtualProtect(m_function_addr, m_length, PAGE_EXECUTE_READWRITE, &original_protection);

	FlushInstructionCache(GetCurrentProcess(), m_function_addr, m_length);

}

