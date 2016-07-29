/* =========================================

		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPRakPeer.hpp
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPRAKPEER_H
#define SAMPRAKPEER_H

#if defined(WIN32)
	#define SYSTEM_MANAGER_OFFSET 8
#elif defined(LINUX)
	#define SYSTEM_MANAGER_OFFSET 4
#endif

#pragma pack(push, 1)
class CSAMPRakPeer
{
	// +0xA = wMaxPlayers
public:
	void *GetRemoteSystem(WORD wPlayerId)
	{
		// Get the remote system manager
		void *pRemoteSystemManager = *(void **)(this + CAddress::OFFSET_RemoteSystemManager + SYSTEM_MANAGER_OFFSET);
		// Get the remote system
		return (void *)(pRemoteSystemManager + CAddress::OFFSET_RemoteSystemSize * wPlayerId);
	}

	void SetRemoteStatus(WORD wPlayerId, PlayerID systemAddress, bool bActive, int iConnectMode, BYTE byteUnknown1, BYTE byteUnknown2)
	{
		// Get the remote system
		void *pRemoteSystem = GetRemoteSystem(wPlayerId);
		// Mark the player as active
		*(bool *)(pRemoteSystem) = bActive;
		// Set his sytem address
		*(PlayerID *)(pRemoteSystem + 1) = systemAddress;
		// Set the connect mode
		*(int *)(pRemoteSystem + CAddress::OFFSET_RemoteSystem__ConnectMode) = iConnectMode;
		*(BYTE *)(pRemoteSystem + CAddress::OFFSET_RemoteSystem__Unknown) = byteUnknown1;
		*(BYTE *)(pRemoteSystem + CAddress::OFFSET_RemoteSystem__Unknown + 1) = byteUnknown2;
	}

	void SetConnectedPlayer(PlayerID systemAddress, WORD wPlayerId)
	{
		SetRemoteStatus(wPlayerId, systemAddress, true, 8, 2, 2);
	};

	void SetDisonnectedPlayer(WORD wPlayerId)
	{
		SetRemoteStatus(wPlayerId, UNASSIGNED_PLAYER_ID, false, 0, 0, 0);
	};

};
#pragma pack(pop)

#endif
