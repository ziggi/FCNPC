/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPRakPeer.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPRAKPEER_H
#define SAMPRAKPEER_H

#pragma pack(push, 1)
class CSAMPSystemAddress
{
	public:
		unsigned int		uiSystemAddress;	// 0x0000 - 0x0004
		unsigned short		usPort;				// 0x0004 - 0x0006
		unsigned short		usPlayerId;			// 0x0006 - 0x0008

		CSAMPSystemAddress& operator = (const CSAMPSystemAddress& systemAddress)
		{
			uiSystemAddress = systemAddress.uiSystemAddress;
			usPort = systemAddress.usPort;
			usPlayerId = systemAddress.usPlayerId;
			return *this;
		};
};
#pragma pack(pop)

#pragma pack(push, 1)
class CSAMPRemoteSystem
{
	public:
		bool					bIsActive;				// 0x0000 - 0x0001
		CSAMPSystemAddress		systemAddress;			// 0x0001 - 0x0009
#ifdef _WIN32
		PAD(pad0, 3239);								// 0x0009 - 0x0CB0
#else
		PAD(pad0, 3161);								// 0x0009 - 0x0C62
#endif
		int						iConnectMode;			// 0x0CB0 - 0x0CB4 / 0x0C62 - 0x0C66
		PAD(pad1, 1);									// 0x0CB4 - 0x0CB5 / 0x0C66 - 0x0C67
		BYTE					byteUnknown;			// 0x0CB5 - 0x0CB6 / 0x0C67 - 0x0C68
		BYTE					byteUnknown2;			// 0x0CB6 - 0x0CB7 / 0x0C68 - 0x0C69
#ifdef _WIN32
		PAD(pad2, 1);									// 0x0CB7 - 0x0CB8
#endif
		// Size = 0xCB8 / 0xC69
};
#pragma pack(pop)

#pragma pack(push, 1)
class CSAMPRemoteSystemManager
{
	public:
		CSAMPRemoteSystem	remoteSystem[MAX_PLAYERS];		
};
#pragma pack(pop)

#pragma pack(push, 1)
class CSAMPRakPeer
{
	public:
		PAD(pad0, 16);											// 0x0000 - 0x0010
		WORD							wMaxPlayers;			// 0x0010 - 0x0012
#ifdef _WIN32
		PAD(pad1, 818);											// 0x0012 - 0x0344
#else
		PAD(pad1, 806);											// 0x0012 - 0x0334
#endif
		CSAMPRemoteSystemManager		*pRemoteSystemManager;	// 0x0344 - 0x0348 / 0x0338 - 0x033C
#ifdef _WIN32
		PAD(pad2, 2768);										// 0x0348 - 0x0E18
#else
		PAD(pad2, 2780);										// 0x033C - 0x0E18
#endif
		// Size = 0xE18;

		void SetConnectedPlayer(CSAMPSystemAddress systemAddress)
		{
			// Mark the player as active
			pRemoteSystemManager->remoteSystem[systemAddress.usPlayerId].bIsActive = true;
			// Set his sytem address
			pRemoteSystemManager->remoteSystem[systemAddress.usPlayerId].systemAddress = systemAddress;
			// Set the connect mode
			pRemoteSystemManager->remoteSystem[systemAddress.usPlayerId].iConnectMode = 8; // 8 = CONNECTED
			pRemoteSystemManager->remoteSystem[systemAddress.usPlayerId].byteUnknown = 2;
			// Disable kicking because they didn't logon to the game
			pRemoteSystemManager->remoteSystem[systemAddress.usPlayerId].byteUnknown2 = 2;
		};

		void SetDisonnectedPlayer(int iPlayerId)
		{
			// Mark the player as inactive
			pRemoteSystemManager->remoteSystem[iPlayerId].bIsActive = false;
			// Set his sytem address
			pRemoteSystemManager->remoteSystem[iPlayerId].systemAddress.uiSystemAddress = 0xFFFFFFFF;
			pRemoteSystemManager->remoteSystem[iPlayerId].systemAddress.usPort = 0xFFFF;
			pRemoteSystemManager->remoteSystem[iPlayerId].systemAddress.usPlayerId = 0;
			pRemoteSystemManager->remoteSystem[iPlayerId].iConnectMode = 0;
			pRemoteSystemManager->remoteSystem[iPlayerId].byteUnknown = 0;
			pRemoteSystemManager->remoteSystem[iPlayerId].byteUnknown2 = 0;
		};

};
#pragma pack(pop)

#endif