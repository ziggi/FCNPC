#include <cstdlib>
#include "ColAndreasDatabaseReader.h"
#include "renderware.h"
using namespace std;

namespace rw 
{
	bool HeaderInfo::read(istream &rw)
	{
		uint32 buf[3];
		rw.read((char*)buf, 12);
		if (rw.eof())
			return false;
		type = buf[0];
		length = buf[1];
		build = buf[2];
		if (build & 0xFFFF0000)
			version = ((build >> 14) & 0x3FF00) |
			((build >> 16) & 0x3F) |
			0x30000;
		else
			version = build << 8;
		return true;
	}

	bool HeaderInfo::peek(istream &rw)
	{
		if (!read(rw))
			return false;
		rw.seekg(-12, ios::cur);
		return true;
	}

	uint32 HeaderInfo::write(ostream &rw)
	{
		uint32 buf[3];
		buf[0] = type;
		buf[1] = length;
		buf[2] = build;
		rw.write((char*)buf, 12);
		return 3 * sizeof(uint32);
	}

	bool HeaderInfo::findChunk(istream &rw, uint32 type)
	{
		while (read(rw)) {
			if (this->type == CHUNK_NAOBJECT)
				return false;
			if (this->type == type)
				return true;
			rw.seekg(length, ios::cur);
		}
		return false;
	}

	void ChunkNotFound(CHUNK_TYPE chunk, uint32 address)
	{
		cerr << "chunk " << hex << chunk << " not found at 0x";
		cerr << hex << address << endl;
		exit(1);
	}

	int8 readInt8(istream &rw)
	{
		int8 tmp;
		rw.read(reinterpret_cast <char *> (&tmp), sizeof(int8));
		return tmp;
	}

	uint8 readUInt8(istream &rw)
	{
		uint8 tmp;
		rw.read(reinterpret_cast <char *> (&tmp), sizeof(uint8));
		return tmp;
	}

	int16 readInt16(istream &rw)
	{
		int16 tmp;
		rw.read(reinterpret_cast <char *> (&tmp), sizeof(int16));
		return tmp;
	}

	uint16 readUInt16(istream &rw)
	{
		uint16 tmp;
		rw.read(reinterpret_cast <char *> (&tmp), sizeof(uint16));
		return tmp;
	}

	int32 readInt32(istream &rw)
	{
		int32 tmp;
		rw.read(reinterpret_cast <char *> (&tmp), sizeof(int32));
		return tmp;
	}

	uint32 readUInt32(istream &rw)
	{
		uint32 tmp;
		rw.read(reinterpret_cast <char *> (&tmp), sizeof(uint32));
		return tmp;
	}

	float32 readFloat32(istream &rw)
	{
		float32 tmp;
		rw.read(reinterpret_cast <char *> (&tmp), sizeof(float32));
		return tmp;
	}

	const char *chunks[] = { "None", "Struct", "String", "Extension", "Unknown",
		"Camera", "Texture", "Material", "Material List", "Atomic Section",
		"Plane Section", "World", "Spline", "Matrix", "Frame List",
		"Geometry", "Clump", "Unknown", "Light", "Unicode String", "Atomic",
		"Texture Native", "Texture Dictionary", "Animation Database",
		"Image", "Skin Animation", "Geometry List", "Anim Animation",
		"Team", "Crowd", "Delta Morph Animation", "Right To Render",
		"MultiTexture Effect Native", "MultiTexture Effect Dictionary",
		"Team Dictionary", "Platform Independet Texture Dictionary",
		"Table of Contents", "Particle Standard Global Data", "AltPipe",
		"Platform Independet Peds", "Patch Mesh", "Chunk Group Start",
		"Chunk Group End", "UV Animation Dictionary", "Coll Tree"
	};

	/* From 0x0101 through 0x0135 */
	const char *toolkitchunks0[] = { "Metrics PLG", "Spline PLG", "Stereo PLG",
		"VRML PLG", "Morph PLG", "PVS PLG", "Memory Leak PLG", "Animation PLG",
		"Gloss PLG", "Logo PLG", "Memory Info PLG", "Random PLG",
		"PNG Image PLG", "Bone PLG", "VRML Anim PLG", "Sky Mipmap Val",
		"MRM PLG", "LOD Atomic PLG", "ME PLG", "Lightmap PLG",
		"Refine PLG", "Skin PLG", "Label PLG", "Particles PLG", "GeomTX PLG",
		"Synth Core PLG", "STQPP PLG",
		"Part PP PLG", "Collision PLG", "HAnim PLG", "User Data PLG",
		"Material Effects PLG", "Particle System PLG", "Delta Morph PLG",
		"Patch PLG", "Team PLG", "Crowd PP PLG", "Mip Split PLG",
		"Anisotrophy PLG", "Not used", "GCN Material PLG", "Geometric PVS PLG",
		"XBOX Material PLG", "Multi Texture PLG", "Chain PLG", "Toon PLG",
		"PTank PLG", "Particle Standard PLG", "PDS PLG", "PrtAdv PLG",
		"Normal Map PLG", "ADC PLG", "UV Animation PLG"
	};

	/* From 0x0180 through 0x01c1 */
	const char *toolkitchunks1[] = {
		"Character Set PLG", "NOHS World PLG", "Import Util PLG",
		"Slerp PLG", "Optim PLG", "TL World PLG", "Database PLG",
		"Raytrace PLG", "Ray PLG", "Library PLG",
		"Not used", "Not used", "Not used", "Not used", "Not used", "Not used",
		"2D PLG", "Tile Render PLG", "JPEG Image PLG", "TGA Image PLG",
		"GIF Image PLG", "Quat PLG", "Spline PVS PLG", "Mipmap PLG",
		"MipmapK PLG", "2D Font", "Intersection PLG", "TIFF Image PLG",
		"Pick PLG", "BMP Image PLG", "RAS Image PLG", "Skin FX PLG",
		"VCAT PLG", "2D Path", "2D Brush", "2D Object", "2D Shape", "2D Scene",
		"2D Pick Region", "2D Object String", "2D Animation PLG",
		"2D Animation",
		"Not used", "Not used", "Not used", "Not used", "Not used", "Not used",
		"2D Keyframe", "2D Maestro", "Barycentric",
		"Platform Independent Texture Dictionary TK", "TOC TK", "TPL TK",
		"AltPipe TK", "Animation TK", "Skin Split Tookit", "Compressed Key TK",
		"Geometry Conditioning PLG", "Wing PLG", "Generic Pipeline TK",
		"Lightmap Conversion TK", "Filesystem PLG", "Dictionary TK",
		"UV Animation Linear", "UV Animation Parameter"
	};

	const char *RSchunks[] = { "Unused 1", "Unused 2", "Unused 3",
		"Pipeline Set", "Unused 5", "Unused 6", "Specular Material",
		"Unused 8", "2dfx", "Night Vertex Colors", "Collision Model",
		"Unused 12", "Reflection Material", "Mesh Extension", "Frame",
		"Unused 16"
	};

	string getChunkName(uint32 i)
	{
		switch (i) {
		case 0x50E:
			return "Bin Mesh PLG";
			break;
		case 0x510:
			return "Native Data PLG";
			break;
		case 0xF21E:
			return "ZModeler Lock";
			break;
		default:
			break;
		}

		if (i <= 45)
			return chunks[i];
		else if (i <= 0x0253F2FF && i >= 0x0253F2F0)
			return RSchunks[i - 0x0253F2F0];
		else if (i <= 0x0135 && i >= 0x0101)
			return toolkitchunks0[i - 0x0101];
		else if (i <= 0x01C0 && i >= 0x0181)
			return toolkitchunks1[i - 0x0181];
		else
			return "Unknown";
	}

}
