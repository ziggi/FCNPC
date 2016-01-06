/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: SAMPRPCParams.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef SAMPRPCPARAMS_H
#define SAMPRPCPARAMS_H

#include <stdio.h>

#pragma pack(push, 1)
class CSAMPRPCParams
{
	private:
		unsigned char			*m_pData;
		DWORD					m_dwLength;
		CSAMPSystemAddress		m_systemAddress;
		// BitStream uses stack data (256 bytes) to store small data instead of heap allocation
		// and since our data is small, we use it
		unsigned char			ucStackData[256];
		int						m_iWritePointer; // Using a write pointer saves us from news and deletes (faster)

	public:
		CSAMPRPCParams()
		{
			// Initialize the data
			m_pData = (unsigned char *)ucStackData;
			m_dwLength = 0;
			m_iWritePointer = 0;
		};

		~CSAMPRPCParams()
		{
			m_dwLength = 0;
		};

		template <typename T>
		void Write(T value, DWORD dwLength = sizeof(T))
		{
			// Write to buffer 
			memcpy(m_pData + m_iWritePointer, &value, dwLength);
			// Set the new length (Multiply by 8 cuz BitStream length is in bits not bytes)
			m_dwLength += (dwLength * 8);
			// Set the write pointer
			m_iWritePointer += dwLength;
		};

		void WriteString(char *szString, DWORD dwLength)
		{
			// Write to buffer (shift left to convert from bits to bytes)
			memcpy(m_pData + m_iWritePointer, szString, dwLength);
			// Set the new length (Multiply by 8 cuz BitStream length is in bits not bytes)
			m_dwLength += (dwLength * 8);
			// Set the write pointer
			m_iWritePointer += dwLength;
		};

		void SetWritePointer(int iWritePointer) 
		{
			// Set the write pointer
			m_iWritePointer = iWritePointer;
			// Set the data length (in bits)
			m_dwLength = iWritePointer * 8;
		};

		void SetSystemAddress(CSAMPSystemAddress systemAddress)
		{
			// Set the RPC params system address
			m_systemAddress = systemAddress;
		};
};
#pragma pack(pop)

#endif