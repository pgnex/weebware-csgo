#pragma once

#ifndef POLYWRAPPER
#define POLYWRAPPER

using namespace PLH;

class CPolyWrapperForVEHHookingBecauseIAmAFatRetardFuckMyLife
{
public:
	VEHHook* Hook;

	auto ProteccLolis()
	{
		return Hook->GetProtectionObject();
	}

	void FBIForce(uintptr_t Src, uintptr_t Dst, VEHHook::VEHMethod TheWay)
	{
		Hook->SetupHook((BYTE*)Src, (BYTE*)Dst, TheWay);
	}

	void LoliCops()
	{
		Hook->Hook();
	}

	template<typename T>
	T GetOriginal()
	{
		return (T)m_ThisCtx.m_Src;
	}

	void JailTime()
	{
		Hook->UnHook();
	}

private:

};

#endif