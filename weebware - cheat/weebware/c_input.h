#pragma once

#include "Header.h"

#define MULTIPLAYER_BACKUP 150

class c_input {
public:
	char pad_0000[0xC];
	bool bTrackIRAvailable;
	bool bMouseInitialized;
	bool bMouseActive;
	char pad_0x08[0x9A];
	bool bCameraInThirdPerson;
	char pad_00C3[0x2];
	Vector vecCameraOffset;
	char pad_00D1[0x38];
	c_usercmd* commands;
	c_verifiedusercmd* verified_commands;
	c_usercmd* get_user_cmd(int sequence_number) {
		typedef c_usercmd* (__thiscall* o_getusercmd)(void*, int, int);
		return &commands[sequence_number % MULTIPLAYER_BACKUP];
	}
	c_usercmd* get_user_cmd(int slot, int sequence_number) {
		typedef c_usercmd* (__thiscall* o_getusercmd)(void*, int, int);
		return getvfunc <o_getusercmd>(this, 8)(this, slot, sequence_number);
	}
	c_verifiedusercmd* get_verified_user_cmd(int sequence_number) {
		return &verified_commands[sequence_number % MULTIPLAYER_BACKUP];
	}
};
