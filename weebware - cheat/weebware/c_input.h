#pragma once

#include "Header.h"

#define MULTIPLAYER_BACKUP 150

class bf_write;
class bf_read;

class c_input
{
public:
    //std::byte			pad0[0xC];				//0x0000
    //bool				bTrackIRAvailable;		//0x000C
    //bool				bMouseInitialized;		//0x000D
    //bool				bMouseActive;			//0x000E
    //std::byte			pad1[0xB2];				//0x000F
    //bool				m_fCameraInThirdPerson;	//0x00C1
    //std::byte			pad2[0x2];				//0x00C2
    //Vector				m_vecCameraOffset;		//0x00C4
    //std::byte			pad3[0x38];				//0x00D0
    //c_usercmd* m_pCommands;				//0x0108
    //c_verifiedusercmd* pVerifiedCommands;		//0x010C

    //c_usercmd* GetUserCmd(const int nSequenceNumber) const
    //{
    //    return &m_pCommands[nSequenceNumber % MULTIPLAYER_BACKUP];
    //}

    //c_verifiedusercmd* GetVerifiedCmd(const int nSequenceNumber) const
    //{
    //    return &pVerifiedCommands[nSequenceNumber % MULTIPLAYER_BACKUP];
    //}

    //c_usercmd* GGetUserCmd(int slot, int sequence_number)
    //{
    //    return &m_pCommands[slot, sequence_number % MULTIPLAYER_BACKUP];
    //}

public:
    char pad0[0xC]; // 0x0
    bool m_trackir_available; // 0xC
    bool m_mouse_initialized; // 0xD
    bool m_mouse_active; // 0xE
    char pad1[0x9E]; // 0xF
    bool m_fCameraInThirdPerson; // 0xAD
    char pad2[0x2]; // 0xAE
    Vector m_vecCameraOffset; // 0xB0
    char pad3[0x38]; // 0xBC
    c_usercmd* m_pCommands; // 0xF4
    c_verifiedusercmd* m_pVerifiedCommands; // 0xF8
    c_usercmd* GetUserCmd(int sequence_number)
    {
        return &m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
    }

    c_verifiedusercmd* GetVerifiedUserCmd(int sequence_number)
    {
        return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
    }
};