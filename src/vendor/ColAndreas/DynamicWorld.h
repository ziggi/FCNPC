#ifndef _INCL_DYNWORLD
#define _INCL_DYNWORLD

#include <btBulletDynamicsCommon.h>
#include "ColObject.h"
#include "Main.hpp"

const btScalar DEG_TO_RAD = btScalar(0.0174532925);
const btScalar RADIAN_TO_DEG = btScalar(57.29577951);

struct ContactCollisionSensor : public btCollisionWorld::ContactResultCallback //collision sensor callback
{
	int collided;
	ContactCollisionSensor() : btCollisionWorld::ContactResultCallback(), collided(0)
	{
	}

	virtual btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1, int index1)
	{
		collided = 1; //if in contact sets the value to 1 else 0
		return 0;
	}
};

class RemovedBuildingManager;
class ObjectManager;
class MapWaterMesh;

class ColAndreasWorld
{
public:
	// Initialize ColAndreas
	ColAndreasWorld();

	// ColAndreas closed
	~ColAndreasWorld();

	RemovedBuildingManager* removedManager;
	ObjectManager* objectManager;

	// Converts GTA rotations to quaternion
	btScalar getDist3D(const btVector3& p1, const btVector3& p2);
	void EulerToQuat(btVector3& rotation, btQuaternion& result);
	void QuatToEuler(btQuaternion& rotation, btVector3& result);
	void setMyExtraID(uint16_t index, int type, int data);
	int getMyExtraID(uint16_t index, int type);
	int performRayTest(const btVector3& Start, const btVector3& End, btVector3& Result, uint16_t& model);
	int performRayTestExtraID(const btVector3& Start, const btVector3& End, btVector3& Result, const int type, uint16_t& data);
	int performRayTestID(const btVector3& Start, const btVector3& End, btVector3& Result, uint16_t& index);
	int performRayTestEx(const btVector3& Start, const btVector3& End, btVector3& Result, btQuaternion& Rotation, btVector3& Position, uint16_t& model);
	int performRayTestAngle(const btVector3& Start, const btVector3& End, btVector3& Result, btScalar& RX, btScalar& RY, btScalar& RZ, uint16_t& model);
	int performRayTestAngleEx(const btVector3& Start, const btVector3& End, btVector3& Result, btScalar& RX, btScalar& RY, btScalar& RZ, btQuaternion& Rotation, btVector3& Position, uint16_t& model);
	int performRayTestAll(const btVector3& Start, const btVector3& End, btAlignedObjectArray < btVector3 >& Result, int ModelIDs[], int size);
	int performRayTestReflection(const btVector3& Start, const btVector3& End, btVector3& Position, btVector3& Result, uint16_t& model);
	int performRayTestNormal(const btVector3& Start, const btVector3& End, btVector3& Result, btVector3& Normal, uint16_t& model);
	int performContactTest(uint16_t modelid, btVector3& objectPos, btQuaternion& objectRot);

	uint16_t createColAndreasMapObject(uint16_t addtomanager, uint16_t modelid, const btQuaternion& objectRot, const btVector3& objectPos);
	uint16_t getModelRef(uint16_t model);
	void colandreasInitMap();
	bool loadCollisionData();

private:
	MapWaterMesh* mapWaterMesh;
	btDynamicsWorld* dynamicsWorld;
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
};

extern ColAndreasWorld* collisionWorld;

#endif
