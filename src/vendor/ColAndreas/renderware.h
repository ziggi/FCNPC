#ifndef _RENDERWARE_H_
#define _RENDERWARE_H_
#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <vector>
#include <string>

#ifdef DEBUG
#define READ_HEADER(x)\
	header.read(rw);\
	if (header.type != (x)) {\
		cerr << filename << " ";\
		ChunkNotFound((x), rw.tellg());\
	}
#else
#define READ_HEADER(x)\
	header.read(rw);
#endif

namespace rw {

	typedef char int8;
	typedef short int16;
	typedef int int32;
	typedef long long int64;
	typedef unsigned char uint8;
	typedef unsigned short uint16;
	typedef unsigned int uint32;
	typedef unsigned long long uint64;
	typedef float float32;

	extern char *filename;
	extern uint32 version;

	enum CHUNK_TYPE {
		CHUNK_NAOBJECT = 0x0,
		CHUNK_STRUCT = 0x1,
		CHUNK_STRING = 0x2,
		CHUNK_EXTENSION = 0x3,
		CHUNK_CAMERA = 0x5,
		CHUNK_TEXTURE = 0x6,
		CHUNK_MATERIAL = 0x7,
		CHUNK_MATLIST = 0x8,
		CHUNK_ATOMICSECT = 0x9,
		CHUNK_PLANESECT = 0xA,
		CHUNK_WORLD = 0xB,
		CHUNK_SPLINE = 0xC,
		CHUNK_MATRIX = 0xD,
		CHUNK_FRAMELIST = 0xE,
		CHUNK_GEOMETRY = 0xF,
		CHUNK_CLUMP = 0x10,
		CHUNK_LIGHT = 0x12,
		CHUNK_UNICODESTRING = 0x13,
		CHUNK_ATOMIC = 0x14,
		CHUNK_TEXTURENATIVE = 0x15,
		CHUNK_TEXDICTIONARY = 0x16,
		CHUNK_ANIMDATABASE = 0x17,
		CHUNK_IMAGE = 0x18,
		CHUNK_SKINANIMATION = 0x19,
		CHUNK_GEOMETRYLIST = 0x1A,
		CHUNK_ANIMANIMATION = 0x1B,
		CHUNK_HANIMANIMATION = 0x1B,
		CHUNK_TEAM = 0x1C,
		CHUNK_CROWD = 0x1D,
		CHUNK_RIGHTTORENDER = 0x1F,
		CHUNK_MTEFFECTNATIVE = 0x20,
		CHUNK_MTEFFECTDICT = 0x21,
		CHUNK_TEAMDICTIONARY = 0x22,
		CHUNK_PITEXDICTIONARY = 0x23,
		CHUNK_TOC = 0x24,
		CHUNK_PRTSTDGLOBALDATA = 0x25,
		CHUNK_ALTPIPE = 0x26,
		CHUNK_PIPEDS = 0x27,
		CHUNK_PATCHMESH = 0x28,
		CHUNK_CHUNKGROUPSTART = 0x29,
		CHUNK_CHUNKGROUPEND = 0x2A,
		CHUNK_UVANIMDICT = 0x2B,
		CHUNK_COLLTREE = 0x2C,
		CHUNK_ENVIRONMENT = 0x2D,
		CHUNK_COREPLUGINIDMAX = 0x2E,

		CHUNK_MORPH = 0x105,
		CHUNK_SKYMIPMAP = 0x110,
		CHUNK_SKIN = 0x116,
		CHUNK_PARTICLES = 0x118,
		CHUNK_HANIM = 0x11E,
		CHUNK_MATERIALEFFECTS = 0x120,
		CHUNK_PDSPLG = 0x131,
		CHUNK_ADCPLG = 0x134,
		CHUNK_UVANIMPLG = 0x135,
		CHUNK_BINMESH = 0x50E,
		CHUNK_NATIVEDATA = 0x510,
		CHUNK_VERTEXFORMAT = 0x510,

		CHUNK_PIPELINESET = 0x253F2F3,
		CHUNK_SPECULARMAT = 0x253F2F6,
		CHUNK_2DFX = 0x253F2F8,
		CHUNK_NIGHTVERTEXCOLOR = 0x253F2F9,
		CHUNK_COLLISIONMODEL = 0x253F2FA,
		CHUNK_REFLECTIONMAT = 0x253F2FC,
		CHUNK_MESHEXTENSION = 0x253F2FD,
		CHUNK_FRAME = 0x253F2FE,
		CHUNK_SAMPCOLLISION = 0x253F2FF
	};
	typedef enum CHUNK_TYPE CHUNK_TYPE;

	struct HeaderInfo
	{
		uint32 type;
		uint32 length;
		uint32 build;
		uint32 version;
		bool read(std::istream &rw);
		bool peek(std::istream &rw);
		uint32 write(std::ostream &rw);
		bool findChunk(std::istream &rw, uint32 type);
	};

	void ChunkNotFound(CHUNK_TYPE chunk, uint32 address);
	int8 readInt8(std::istream &rw);
	uint8 readUInt8(std::istream &rw);
	int16 readInt16(std::istream &rw);
	uint16 readUInt16(std::istream &rw);
	int32 readInt32(std::istream &rw);
	uint32 readUInt32(std::istream &rw);
	float32 readFloat32(std::istream &rw);

	std::string getChunkName(uint32 i);

	struct ColHeader
	{
		char validator[3];
		char version;
		unsigned int size;
		char modelName[22];
		short modelid;
	};

	struct ColBox
	{
		Vector box_min;
		Vector box_max;
	};

	struct ColItems
	{
		uint16_t numSpheres;
		uint16_t numBoxes;
		uint16_t numFaces;
		uint8_t numWheels;
		uint8_t padding;
		uint32_t flags;
		uint32_t offSpheres;
		uint32_t offBoxes;
		uint32_t offSuspensionLines;
		uint32_t offVertices;
		uint32_t offFaces;
		uint32_t offTrianglePlanes;

		uint32_t numShadow;
		uint32_t offShadowVert;
		uint32_t offShadowFaces;
	};

	struct Collision
	{
		bool read(std::istream &dff, int32_t modelid);
	};

	struct Clump
	{
		int32_t iModelId = 0;
		Collision collision;

		/* Extensions */
		/* collision file */
		bool hasCollision;

		/* functions */
		bool read(std::istream &dff, int32_t modelid);
		bool readExtension(std::istream &dff);
	};

}

#endif
