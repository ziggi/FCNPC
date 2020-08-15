#include <cmath>
#include "ColAndreasCommon.h"
#include "DynamicWorld.h"
#include "renderware.h"
using namespace std;

namespace rw {

	char *filename;

	/*
	 * Clump
	 */

	bool Clump::read(istream& rw, int32_t modelid)
	{
		iModelId = modelid;
		HeaderInfo header;
		header.read(rw);

		READ_HEADER(CHUNK_STRUCT);
		uint32 numAtomics = readUInt32(rw);
		uint32 numLights = 0;
		if (header.length == 0xC) {
			numLights = readUInt32(rw);
			rw.seekg(4, ios::cur);
		}

		// skip framelist
		READ_HEADER(CHUNK_FRAMELIST);
		rw.seekg(header.length, ios::cur);

		// skip geometry list
		READ_HEADER(CHUNK_GEOMETRYLIST);
		rw.seekg(header.length, ios::cur);

		// skip atomic
		READ_HEADER(CHUNK_ATOMIC);
		rw.seekg(header.length, ios::cur);

		// skip lights
		for (uint32 i = 0; i < numLights; i++) {
			READ_HEADER(CHUNK_STRUCT);
			rw.seekg(header.length, ios::cur);
		}

		hasCollision = false;
		return readExtension(rw);
	}

	bool Clump::readExtension(istream &rw)
	{
		bool result = false;
		HeaderInfo header;

		READ_HEADER(CHUNK_EXTENSION);
		streampos end = rw.tellg();
		end += header.length;

		while (rw.tellg() < end) {
			header.read(rw);
			switch (header.type) {
			case CHUNK_SAMPCOLLISION:
				hasCollision = true;
				result = collision.read(rw, iModelId);
				break;
			default:
				return result;
			}
		}
		return result;
	}

	bool Collision::read(std::istream &rw, int32_t modelid)
	{
		ColHeader fileHeader;
		std::streamoff beginPos = rw.tellg();
		rw.read((char*)&fileHeader, sizeof(fileHeader));
		if (rw.gcount() == sizeof(fileHeader) && !strncmp(fileHeader.validator, "COL3", 4))
		{
			rw.seekg(40, std::ios::cur);
			if (ModelCount > 0)
			{
				uint16_t i = ModelCount;
				ModelCount++;

				CollisionModels[i].CustomModelid = modelid;
				if (fileHeader.version == 'L') // Version 1
				{
					uint32_t itemCount = 0;

					// Spheres
					rw.read((char*)&itemCount, sizeof(itemCount));
					if (rw.gcount() != sizeof(itemCount)) return false;

					if (itemCount)
					{
						CollisionModels[i].SphereCount = itemCount;
						CollisionModels[i].SphereData = new structSphereData[CollisionModels[i].SphereCount];

						for (int j = 0; j < itemCount; ++j)
						{
							rw.read((char*) &(CollisionModels[i].SphereData[j].Radius), sizeof(float));
							rw.read((char*) &(CollisionModels[i].SphereData[j].Offset), sizeof(Vector));
							rw.seekg(4, std::ios::cur); // Skip surface data
						}
					}
					else CollisionModels[i].SphereData = NULL;

					rw.seekg(4, std::ios::cur); // Skip unknown data

					// Boxes
					rw.read((char*)&itemCount, sizeof(itemCount));
					if (rw.gcount() != sizeof(itemCount)) return false;

					if (itemCount)
					{
						CollisionModels[i].BoxCount = itemCount;
						CollisionModels[i].BoxData = new structBoxData[CollisionModels[i].BoxCount];
						ColBox *boxTemp = new ColBox[CollisionModels[i].BoxCount];

						for (int j = 0; j < itemCount; ++j)
						{
							rw.read((char*)&boxTemp[j], sizeof(ColBox));

							CollisionModels[i].BoxData[j].Center.x = (boxTemp[j].box_min.x + boxTemp[j].box_max.x) / 2.0f;
							CollisionModels[i].BoxData[j].Center.y = (boxTemp[j].box_min.y + boxTemp[j].box_max.y) / 2.0f;
							CollisionModels[i].BoxData[j].Center.z = (boxTemp[j].box_min.z + boxTemp[j].box_max.z) / 2.0f;

							CollisionModels[i].BoxData[j].Size.x = (boxTemp[j].box_max.x - boxTemp[j].box_min.x) / 2.0f;
							CollisionModels[i].BoxData[j].Size.y = (boxTemp[j].box_max.y - boxTemp[j].box_min.y) / 2.0f;
							CollisionModels[i].BoxData[j].Size.z = (boxTemp[j].box_max.z - boxTemp[j].box_min.z) / 2.0f;

							rw.seekg(4, std::ios::cur);
						}
					}
					else CollisionModels[i].BoxData = NULL;

					// Vertices
					rw.read((char*)&itemCount, sizeof(itemCount));
					if (rw.gcount() != sizeof(itemCount)) return false;

					if (itemCount)
					{
						Vector *vertices = new Vector[itemCount];
						rw.read((char*)vertices, sizeof(Vector) * itemCount);

						// Faces
						rw.read((char*)&itemCount, sizeof(itemCount));
						if (rw.gcount() != sizeof(itemCount))
						{
							delete[] vertices;
							return false;
						}

						if (itemCount)
						{
							CollisionModels[i].FaceCount = itemCount;
							CollisionModels[i].FacesData = new structFacesData[CollisionModels[i].FaceCount];


							for (int j = 0; j < itemCount; ++j)
							{
								uint32_t indexes[3];
								rw.read((char*)&indexes, sizeof(indexes));
								rw.seekg(4, std::ios::cur); // Skip surface data

								CollisionModels[i].FacesData[j].FaceA = vertices[indexes[0]];
								CollisionModels[i].FacesData[j].FaceB = vertices[indexes[1]];
								CollisionModels[i].FacesData[j].FaceC = vertices[indexes[2]];
							}
						}
						else
						{
							delete[] vertices;
							CollisionModels[i].FacesData = NULL;
						}
					}
					else CollisionModels[i].FacesData = NULL;

					// Prepare for reading the next collision
					rw.seekg(beginPos + 8 + fileHeader.size);

					ModelRef[CollisionModels[i].Modelid] = i;
					ColAndreasColObject* colObject = new ColAndreasColObject(i, false);
					ColAndreasColObject* convex = new ColAndreasColObject(i, true);
					colObjects.push_back(colObject);
					colConvex.push_back(convex->getCompoundShape());
					return true;
				}
				else
				{
					ColItems fileItems;
					rw.read((char*)&fileItems, sizeof(fileItems));

					if (rw.gcount() == sizeof(fileItems))
					{
						CollisionModels[i].BoxCount = fileItems.numBoxes;
						CollisionModels[i].SphereCount = fileItems.numSpheres;
						CollisionModels[i].FaceCount = fileItems.numFaces;

						// Box Data
						if (fileItems.numBoxes)
						{
							CollisionModels[i].BoxData = new structBoxData[fileItems.numBoxes];
							ColBox *boxTemp = new ColBox[fileItems.numBoxes];
							rw.seekg(beginPos + 4 + fileItems.offBoxes);

							for (int j = 0; j < fileItems.numBoxes; ++j)
							{
								rw.read((char*)&boxTemp[j], sizeof(ColBox));

								CollisionModels[i].BoxData[j].Center.x = (boxTemp[j].box_min.x + boxTemp[j].box_max.x) / 2.0f;
								CollisionModels[i].BoxData[j].Center.y = (boxTemp[j].box_min.y + boxTemp[j].box_max.y) / 2.0f;
								CollisionModels[i].BoxData[j].Center.z = (boxTemp[j].box_min.z + boxTemp[j].box_max.z) / 2.0f;

								CollisionModels[i].BoxData[j].Size.x = (boxTemp[j].box_max.x - boxTemp[j].box_min.x) / 2.0f;
								CollisionModels[i].BoxData[j].Size.y = (boxTemp[j].box_max.y - boxTemp[j].box_min.y) / 2.0f;
								CollisionModels[i].BoxData[j].Size.z = (boxTemp[j].box_max.z - boxTemp[j].box_min.z) / 2.0f;

								rw.seekg(4, std::ios::cur); // Skip surface data
							}
						}
						else CollisionModels[i].BoxData = NULL;

						if (fileItems.numSpheres)
						{
							CollisionModels[i].SphereData = new structSphereData[fileItems.numSpheres];
							rw.seekg(beginPos + 4 + fileItems.offSpheres);

							for (int j = 0; j < fileItems.numSpheres; ++j)
							{
								rw.read((char*)&CollisionModels[i].SphereData[j], sizeof(structSphereData));
								rw.seekg(4, std::ios::cur); // Skip surface data
							}
						}
						else CollisionModels[i].SphereData = NULL;

						if (fileItems.numFaces)
						{
							uint16_t topIndex = 0;
							uint16_t(*faceIndexes)[3] = new uint16_t[fileItems.numFaces][3];

							CollisionModels[i].FacesData = new structFacesData[CollisionModels[i].FaceCount];
							rw.seekg(beginPos + 4 + fileItems.offFaces);

							for (int j = 0; j < fileItems.numFaces; ++j)
							{
								rw.read((char*)(faceIndexes + j), sizeof(uint16_t) * 3);
								rw.seekg(2, std::ios::cur); // Skip surface data

								if (faceIndexes[j][0] > topIndex) topIndex = faceIndexes[j][0];
								if (faceIndexes[j][1] > topIndex) topIndex = faceIndexes[j][1];
								if (faceIndexes[j][2] > topIndex) topIndex = faceIndexes[j][2];
							}

							Vector *vertices = new Vector[topIndex + 1];
							rw.seekg(beginPos + 4 + fileItems.offVertices);

							for (int j = 0; j < topIndex + 1; ++j)
							{
								int16_t vertexData[3];
								rw.read((char*)&vertexData, sizeof(vertexData));

								vertices[j].x = float(vertexData[0]) / 128.0f;
								vertices[j].y = float(vertexData[1]) / 128.0f;
								vertices[j].z = float(vertexData[2]) / 128.0f;
							}

							for (int j = 0; j < fileItems.numFaces; ++j)
							{

								CollisionModels[i].FacesData[j].FaceA = (vertices[faceIndexes[j][0]]);
								CollisionModels[i].FacesData[j].FaceB = (vertices[faceIndexes[j][1]]);
								CollisionModels[i].FacesData[j].FaceC = (vertices[faceIndexes[j][2]]);
							}

							delete[] faceIndexes;
						}
						else CollisionModels[i].FacesData = NULL;

						// Prepare for reading the next collision
						rw.seekg(beginPos + 8 + fileHeader.size);

						ModelRef[CollisionModels[i].CustomModelid] = i;

						ColAndreasColObject* colObject = new ColAndreasColObject(i, false);
						ColAndreasColObject* convex = new ColAndreasColObject(i, true);
						colObjects.push_back(colObject);
						colConvex.push_back(convex->getCompoundShape());
						return true;
					}
				}
			}
		}
		return false;
	}
}
