/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SANode.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SANODE_H
#define SANODE_H

// Header
#pragma pack(push, 1)
class CSANodeHeader
{
	public:
		unsigned long		ulNodesNumber;				// 0x0000 - 0x0004
		unsigned long		ulVehicleNodesNumber;		// 0x0004 - 0x0008
		unsigned long		ulPedNodesNumber;			// 0x0008 - 0x000C
		unsigned long		ulNaviNodesNumber;			// 0x000C - 0x0010
		unsigned long		ulLinksNumber;				// 0x0010 - 0x0014
		// Size = 0x14
};
#pragma pack(pop)

// Path Node
#pragma pack(push, 1)
class CSAPathNode
{
	public:
		unsigned long		ulMemAddress;				// 0x0000 - 0x0004
		unsigned long		ulUnknown1;					// 0x0004 - 0x0008
		short				sPositionX;					// 0x0008 - 0x000A
		short				sPositionY;					// 0x000A - 0x000C
		short				sPositionZ;					// 0x000C - 0x000E
		short				sUnknown2;					// 0x000E - 0x0010
		unsigned short		usLinkId;					// 0x0010 - 0x0012
		unsigned short		usAreaId;					// 0x0012 - 0x0014
		unsigned short		usNodeId;					// 0x0014 - 0x0016
		unsigned char		ucPathWidth;				// 0x0016 - 0x0017
		unsigned char		ucNodeType;					// 0x0017 - 0x0018
		unsigned long		ulFlags;					// 0x0018 - 0x001C
		// Size = 0x1C
};
#pragma pack(pop)

// Navi Node
#pragma pack(push, 1)
class CSANaviNode
{
	public:
		short				sPositionX;					// 0x0000 - 0x0002
		short				sPositionY;					// 0x0002 - 0x0004
		unsigned short		usAreaId;					// 0x0004 - 0x0006
		unsigned short		usNodeId;					// 0x0006 - 0x0008
		unsigned char		ucDirectionX;				// 0x0008 - 0x0009
		unsigned char		ucDirectionY;				// 0x0009 - 0x000A
		unsigned long		ulFlags;					// 0x000A - 0x000E
		// Size = 0x0E
};
#pragma pack(pop)

// Link Node
#pragma pack(push, 1)
class CSALinkNode
{
	public:
		unsigned short		usAreaId;					// 0x0000 - 0x0002
		unsigned short		usNodeId;					// 0x0002 - 0x0004
		// Size = 0x4
};
#pragma pack(pop)

#endif
