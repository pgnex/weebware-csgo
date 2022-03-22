#pragma once

#include "Header.h"

#define MULTIPLAYER_BACKUP 150

class c_input
{
public:
	std::byte			pad0[0xC];				//0x00
	bool				bTrackIRAvailable;		//0x0C
	bool				bMouseInitialized;		//0x0D
	bool				bMouseActive;			//0x0E
	std::byte			pad1[0x9E];				//0x0F
	bool				bCameraInThirdPerson;	//0xAD
	std::byte			pad2[0x2];				//0xAE
	Vector				vecCameraOffset;		//0xB0
	std::byte			pad3[0x38];				//0xBC
	c_usercmd* pCommands;				//0xF4
	c_verifiedusercmd* pVerifiedCommands;		//0xF8

    c_usercmd* GetUserCmd(const int sequence_number)
    {
        return &pCommands[sequence_number % MULTIPLAYER_BACKUP];
    }

    c_verifiedusercmd* GetVerifiedUserCmd(const int sequence_number)
    {
        return &pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
    }
};
static_assert(sizeof(c_input) == 0xFC);
