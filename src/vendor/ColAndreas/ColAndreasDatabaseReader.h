#ifndef COLANDREASDATABASEREADER_H_INCLUDED
#define COLANDREASDATABASEREADER_H_INCLUDED

#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

/*
Binary file format


char[4] Should be "Cskp"
which stands for Chris, Slice, Kanada, Pottus


unsigned intiger: number of collision models
unsigned intiger: number of IPL entries


(CollisionDataBlock * number of collision models)

unsigned int: modelid
unsigned int: number of spheres
unsigned int: number of boxes
unsigned int: number of faces

(SphereData * number of spheres)
float: x,y,z, radius

(BoxData * number of boxes)
float: center: x,y,z
float Size: x,y,z

(FacesData * number of faces)
float: face A: x,y,z
float: face B: x,y,z
float: face C: x,y,z

(IPLDataBlock * number of IPL entries)
unsigned int: modelid
float: position: x,y,z
float: rotation: x,y,z,w
*/

//Vector struct, used in models and IPLs

typedef struct {
	float x, y, z;
} Vector;

typedef struct {
	float x, y, z, w;
} Quaternion;
//define the structures

typedef struct {
	Vector Offset;
	float Radius;
} structSphereData;

typedef struct {
	Vector Center;
	Vector Size;
} structBoxData;

typedef struct {
	Vector FaceA;
	Vector FaceB;
	Vector FaceC;
} structFacesData;
//ColAndreas database file structure
typedef struct
{
	uint16_t Modelid;

	//define our counts
	uint16_t SphereCount;
	uint16_t BoxCount;
	uint16_t FaceCount;

	//uninitialized arrays of structured data
	structSphereData* SphereData;
	structBoxData* BoxData;
	structFacesData* FacesData;

} CollisionModelstructure;

typedef struct
{
	uint16_t Modelid;//2 bytes

	Vector Position;//12 bytes
	Quaternion Rotation;//16 bytes
} ItemPlacementstructure;//30 bytes total

#define GetBytes(Source, Destination, Index, BytesToCopy) memcpy(&(Destination), (Source + Index), BytesToCopy), Index+=BytesToCopy

//function to read database file
/*
pass a string, which is the directory location of the database file
pass a refrence to the collisionmodel and itemplacment array's. The function will set the data.

returns a boolean true if function suceeded, otherwise false.
*/

bool ReadColandreasDatabaseFile(std::string FileLocation);
extern CollisionModelstructure* CollisionModels;
extern ItemPlacementstructure* ModelPlacements;
extern uint16_t ModelCount;
extern uint32_t IPLCount;
extern uint16_t ModelRef[20000];

#endif 

