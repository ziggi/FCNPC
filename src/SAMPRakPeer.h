/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPRakPeer.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPRAKPEER_H
#define SAMPRAKPEER_H

class CSAMPSystemAddress;

#ifdef _WIN32
	#define SYSTEM_MANAGER_OFFSET 8
#else
	#define SYSTEM_MANAGER_OFFSET 4
#endif

class CSAMPRemoteSystem {};
class CSAMPRemoteSystemManager{};

#pragma pack(push, 1)
class CSAMPRakPeer
{
	// +0xA = wMaxPlayers	
	public:
		CSAMPRemoteSystem *GetRemoteSystem(unsigned short usIndex)
		{
			// Get the remote system manager
			CSAMPRemoteSystemManager *pRemoteSystemManager = *(CSAMPRemoteSystemManager **)(this + CAddress::OFFSET_RemoteSystemManager + SYSTEM_MANAGER_OFFSET);
			// Get the remote system
			return (CSAMPRemoteSystem *)(pRemoteSystemManager + CAddress::OFFSET_RemoteSystemSize * usIndex);
		}

		void SetConnectedPlayer(PlayerID systemAddress, int iPlayerId)
		{
			// Get the remote system
			CSAMPRemoteSystem *pRemoteSystem = GetRemoteSystem((unsigned short)iPlayerId);
			// Mark the player as active
			*(bool *)(pRemoteSystem) = true;
			// Set his sytem address
			*(PlayerID *)(pRemoteSystem + 1) = systemAddress;
			// Set the connect mode
			*(int *)(pRemoteSystem + CAddress::OFFSET_RemoteSystem__ConnectMode) = 8; // 8 = CONNECTED
			*(BYTE *)(pRemoteSystem + CAddress::OFFSET_RemoteSystem__Unknown) = 2;
			*(BYTE *)(pRemoteSystem + CAddress::OFFSET_RemoteSystem__Unknown + 1) = 2;
		};

		void SetDisonnectedPlayer(int iPlayerId)
		{
			// Get the remote system
			CSAMPRemoteSystem *pRemoteSystem = GetRemoteSystem((unsigned short)iPlayerId);
			// Mark the player as inactive
			*(bool *)(pRemoteSystem) = false;
			// Reset stats
			*(PlayerID *)(pRemoteSystem + 1) = PlayerID();
			*(int *)(pRemoteSystem + CAddress::OFFSET_RemoteSystem__ConnectMode) = 0;
			*(BYTE *)(pRemoteSystem + CAddress::OFFSET_RemoteSystem__Unknown) = 0;
			*(BYTE *)(pRemoteSystem + CAddress::OFFSET_RemoteSystem__Unknown + 1) = 0;
		};

};
#pragma pack(pop)

#endif
