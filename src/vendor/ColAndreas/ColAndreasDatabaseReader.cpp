#include "ColAndreasDatabaseReader.h"
#include "Main.hpp"

CollisionModelstructure* CollisionModels;
ItemPlacementstructure* ModelPlacements;

uint16_t ModelCount = 0;
uint32_t IPLCount = 0;
uint16_t ModelRef[20000];

void DeleteCollisionData()
{
	delete CollisionModels;
	delete ModelPlacements;
}

bool ReadColandreasDatabaseFile(std::string FileLocation)
{
	bool returnValue = false;

	ifstream ColAndreasBinaryfile;

	ColAndreasBinaryfile.open(FileLocation, ios::in | ios::binary);

	if (ColAndreasBinaryfile.is_open()) {
		ColAndreasBinaryfile.seekg(0, ColAndreasBinaryfile.end);
		int length = static_cast<int>(ColAndreasBinaryfile.tellg());

		ColAndreasBinaryfile.seekg(0, ColAndreasBinaryfile.beg);

		char * buffer = new char[length];

		ColAndreasBinaryfile.read(buffer, length);

		uint32_t FileIndex = 0;

		char FileExtension[4];
		GetBytes(buffer, FileExtension, FileIndex, 4);
		
		//If is a ColAndreas binary file.
		if (!strncmp(FileExtension, "cadf", 4))
		{
			uint16_t fileVersion;			
			GetBytes(buffer, fileVersion, FileIndex, 2);
			
			if(fileVersion == CA_DATABASE_VERSION)
			{
				GetBytes(buffer, ModelCount, FileIndex, 2);
				GetBytes(buffer, IPLCount, FileIndex, 4);

				if (ModelCount > 0) {
					CollisionModels = new CollisionModelstructure[ModelCount];

					for (uint16_t i = 0; i < ModelCount; i++) {
						GetBytes(buffer, CollisionModels[i].Modelid, FileIndex, 2);
						GetBytes(buffer, CollisionModels[i].SphereCount, FileIndex, 2);
						GetBytes(buffer, CollisionModels[i].BoxCount, FileIndex, 2);
						GetBytes(buffer, CollisionModels[i].FaceCount, FileIndex, 2);

						if (CollisionModels[i].SphereCount > 0) {
							CollisionModels[i].SphereData = new structSphereData[CollisionModels[i].SphereCount];

							for (uint16_t j = 0; j < CollisionModels[i].SphereCount; j++) {
								GetBytes(buffer, CollisionModels[i].SphereData[j], FileIndex, sizeof(structSphereData));
							}
						}

						if (CollisionModels[i].BoxCount > 0) {
							CollisionModels[i].BoxData = new structBoxData[CollisionModels[i].BoxCount];

							for (uint16_t j = 0; j < CollisionModels[i].BoxCount; j++) {
								GetBytes(buffer, CollisionModels[i].BoxData[j], FileIndex, sizeof(structBoxData));
							}
						}

						if (CollisionModels[i].FaceCount > 0)
						{
							CollisionModels[i].FacesData = new structFacesData[CollisionModels[i].FaceCount];

							for (uint16_t j = 0; j < CollisionModels[i].FaceCount; j++) {
								GetBytes(buffer, CollisionModels[i].FacesData[j], FileIndex, sizeof(structFacesData));
							}
						}
					}
				}
				
				if (IPLCount > 0) {
					ModelPlacements = new ItemPlacementstructure[IPLCount];

					for (uint32_t i = 0; i < IPLCount; i++) {
						GetBytes(buffer, ModelPlacements[i].Modelid, FileIndex, sizeof(uint16_t));
						GetBytes(buffer, ModelPlacements[i].Position, FileIndex, sizeof(Vector));
						GetBytes(buffer, ModelPlacements[i].Rotation, FileIndex, sizeof(Quaternion));
					}
				}

				// Set model ref default values
				for (int i = 0; i < 20000; i++)
				{
					ModelRef[i] = 65535;
				}

				// Initialize model reference
				for (int i = 0; i < ModelCount; i++)
				{
					ModelRef[CollisionModels[i].Modelid] = i;
				}

				returnValue = true;
			}
			else
			{
				logprintf("ERROR: Incompatible database file, expecting version 0x%04X, but found 0x%04X.", CA_DATABASE_VERSION, fileVersion);
				returnValue = false;
			}
		}
		
		delete [] buffer;
	}
	ColAndreasBinaryfile.close();
	return returnValue;
}