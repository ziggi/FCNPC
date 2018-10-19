#include "DynamicWorld.h"

// Initialize ColAndreas
ColAndreasWorld::ColAndreasWorld()
{
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	removedManager = new RemovedBuildingManager();
	objectManager = new ObjectManager();
}

// ColAndreas closed
ColAndreasWorld::~ColAndreasWorld()
{
	if (objectManager != NULL)
		delete removedManager;

	if (removedManager != NULL)
		delete removedManager;

	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;

	if (mapWaterMesh != NULL)
		delete mapWaterMesh;
}

btScalar ColAndreasWorld::getDist3D(const btVector3& c1, const btVector3& c2)
{
	btScalar dx = c2.getX() - c1.getX();
	btScalar dy = c2.getY() - c1.getY();
	btScalar dz = c2.getZ() - c1.getZ();
	return sqrt((btScalar)(dx * dx + dy * dy + dz * dz));
}

	// Converts GTA rotations to quaternion
void ColAndreasWorld::EulerToQuat(btVector3& rotation, btQuaternion& result)
{
	rotation.setX(rotation.getX() * DEG_TO_RAD);
	rotation.setY(rotation.getY() * DEG_TO_RAD);
	rotation.setZ(rotation.getZ() * DEG_TO_RAD);

	btScalar c1 = cos(rotation.getY() / 2);
	btScalar s1 = sin(rotation.getY() / 2);
	btScalar c2 = cos(rotation.getX() / 2);
	btScalar s2 = sin(rotation.getX() / 2);
	btScalar c3 = cos(rotation.getZ() / 2);
	btScalar s3 = sin(rotation.getZ() / 2);
	btScalar c1c2 = c1*c2;
	btScalar s1s2 = s1*s2;
	result.setW((c1c2*c3 - s1s2*s3));
	result.setZ(c1c2*s3 + s1s2*c3);
	result.setY(s1*c2*c3 + c1*s2*s3);
	result.setX(c1*s2*c3 - s1*c2*s3);
}

void ColAndreasWorld::QuatToEuler(btQuaternion& rotation, btVector3& result)
{
	result.setY((-asin(2 * ((rotation.getX() * rotation.getZ()) + (rotation.getW() * rotation.getY()))) * RADIAN_TO_DEG) );
	result.setX((atan2(2 * ((rotation.getY() * rotation.getZ()) + (rotation.getW() * rotation.getX())), (rotation.getW() * rotation.getW()) - (rotation.getX() * rotation.getX()) - (rotation.getY() * rotation.getY()) + (rotation.getZ() * rotation.getZ())) * RADIAN_TO_DEG) );
	result.setZ((-atan2(2 * ((rotation.getX() * rotation.getY()) + (rotation.getW() * rotation.getZ())), (rotation.getW() * rotation.getW()) + (rotation.getX() * rotation.getX()) - (rotation.getY() * rotation.getY()) - (rotation.getZ() * rotation.getZ())) * RADIAN_TO_DEG) );
}

int ColAndreasWorld::performRayTest(const btVector3& Start, const btVector3& End, btVector3& Result, uint16_t& model)
{
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	dynamicsWorld->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit())
	{
		Result = RayCallback.m_hitPointWorld;
		model = RayCallback.m_collisionObject->getUserIndex();
		return 1;
	}
	return 0;
}

int ColAndreasWorld::performRayTestExtraID(const btVector3& Start, const btVector3& End, btVector3& Result, const int type, uint16_t& data)
{
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	dynamicsWorld->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit())
	{
		Result = RayCallback.m_hitPointWorld;
		if(type >= 0 && type < 10)
		{
			ColAndreasObjectTracker* tracker = (ColAndreasObjectTracker*)RayCallback.m_collisionObject->getUserPointer();
			data = tracker->extraData[type];
		}
		else
			data = -1;
		return 1;
	}
	return 0;
}

int ColAndreasWorld::performRayTestID(const btVector3& Start, const btVector3& End, btVector3& Result, uint16_t& index)
{
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	dynamicsWorld->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit())
	{
		Result = RayCallback.m_hitPointWorld;
		ColAndreasObjectTracker* tracker = (ColAndreasObjectTracker*)RayCallback.m_collisionObject->getUserPointer();
		index = tracker->realIndex;
		return 1;
	}
	return 0;
}

int ColAndreasWorld::performRayTestEx(const btVector3& Start, const btVector3& End, btVector3& Result, btQuaternion& Rotation, btVector3& Position, uint16_t& model)
{
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	dynamicsWorld->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit())
	{
		Result = RayCallback.m_hitPointWorld;
		model = RayCallback.m_collisionObject->getUserIndex();
		Rotation = RayCallback.m_collisionObject->getWorldTransform().getRotation();
		Position = RayCallback.m_collisionObject->getWorldTransform().getOrigin();
		return 1;
	}
	return 0;
}


int ColAndreasWorld::performRayTestAngle(const btVector3& Start, const btVector3& End, btVector3& Result, btScalar& RX, btScalar& RY, btScalar& RZ, uint16_t& model)
{
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	dynamicsWorld->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit())
	{
		btVector3 Rotation = RayCallback.m_hitNormalWorld;
		RX = -(asin(Rotation.getY())*RADIAN_TO_DEG);
		RY = asin(Rotation.getX())*RADIAN_TO_DEG;
		// I think there is a way to calculate this not sure how yet
		RZ = 0.0;
		Result = RayCallback.m_hitPointWorld;
		model = RayCallback.m_collisionObject->getUserIndex();
		return 1;
	}
	return 0;
}

int ColAndreasWorld::performRayTestAngleEx(const btVector3& Start, const btVector3& End, btVector3& Result, btScalar& RX, btScalar& RY, btScalar& RZ, btQuaternion& Rotation, btVector3& Position, uint16_t& model)
{
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	dynamicsWorld->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit())
	{
		btVector3 Normal = RayCallback.m_hitNormalWorld;
		

		RX = -(asin(Normal.getY())*RADIAN_TO_DEG);
		RY = asin(Normal.getX())*RADIAN_TO_DEG;
		// There is a way to calculate this not sure how yet
		RZ = 0.0;
		Result = RayCallback.m_hitPointWorld;


		model = RayCallback.m_collisionObject->getUserIndex();
		Rotation = RayCallback.m_collisionObject->getWorldTransform().getRotation();
		Position = RayCallback.m_collisionObject->getWorldTransform().getOrigin();
		return 1;
	}
	return 0;
}


int ColAndreasWorld::performRayTestAll(const btVector3& Start, const btVector3& End, btAlignedObjectArray < btVector3 >& Result, int ModelIDs[], int size)
{
	btCollisionWorld::AllHitsRayResultCallback RayCallback(Start, End);

	dynamicsWorld->rayTest(Start, End, RayCallback);


	if (RayCallback.hasHit())
	{
		if (RayCallback.m_hitPointWorld.size() <= size)
		{
			for (int i = 0; i < RayCallback.m_hitPointWorld.size(); i++)
			{
				ModelIDs[i] = RayCallback.m_collisionObjects[i]->getUserIndex();

			}

			Result = RayCallback.m_hitPointWorld;
			return 1;
		}
	}
	return 0;
}

// Return reflection vector
int ColAndreasWorld::performRayTestReflection(const btVector3& Start, const btVector3& End, btVector3& Position, btVector3& Result, uint16_t& model)
{
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	dynamicsWorld->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit())
	{
		Position = RayCallback.m_hitPointWorld;
		model = RayCallback.m_collisionObject->getUserIndex();

		// Calculates the reflection vector of the given Raycast
		btVector3 Normal = RayCallback.m_hitNormalWorld;
		btScalar Magnitude = this->getDist3D(Start, Position);
		btVector3 UVector = (Position - Start) / btVector3(Magnitude, Magnitude, Magnitude);
		Result = UVector - 2 * UVector.dot(Normal) * Normal;
		
		return 1;
	}
	return 0;
}


int ColAndreasWorld::performRayTestNormal(const btVector3& Start, const btVector3& End, btVector3& Result, btVector3& Normal, uint16_t& model)
{
	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

	dynamicsWorld->rayTest(Start, End, RayCallback);

	if (RayCallback.hasHit())
	{
		Normal = RayCallback.m_hitNormalWorld;
		Result = RayCallback.m_hitPointWorld;
		model = RayCallback.m_collisionObject->getUserIndex();
		return 1;
	}
	return 0;
}

int ColAndreasWorld::performContactTest(uint16_t modelid, btVector3& objectPos, btQuaternion& objectRot)
{
	ContactCollisionSensor callback;
	
	uint16_t colindex = ModelRef[modelid];
	btDefaultMotionState* colMapObjectPosition = new btDefaultMotionState(btTransform(objectRot, objectPos));
	btRigidBody::btRigidBodyConstructionInfo meshRigidBodyCI(0, colMapObjectPosition, colConvex[colindex], btVector3(0, 0, 0));
	btRigidBody* colMapRigidBody = new btRigidBody(meshRigidBodyCI);
	
	dynamicsWorld->contactTest(colMapRigidBody, callback);
	
	delete colMapRigidBody->getMotionState();
	delete colMapRigidBody;
	
	return callback.collided;
}

void ColAndreasWorld::colandreasInitMap()
{
	// Create water map mesh
	mapWaterMesh = new MapWaterMesh(this->dynamicsWorld);

	// Create all map object
	InitCollisionMap(this->dynamicsWorld, this->removedManager);
}

uint16_t ColAndreasWorld::createColAndreasMapObject(uint16_t addtomanager, uint16_t modelid, const btQuaternion& objectRot, const btVector3& objectPos)
{
	ColAndreasMapObject* mapObject = new ColAndreasMapObject(modelid, objectRot, objectPos, this->dynamicsWorld);
	if (addtomanager)
	{
		uint16_t index = 0;
		return this->objectManager->addObjectManager(mapObject);
	}
	return -1;
}

uint16_t ColAndreasWorld::getModelRef(uint16_t model)
{
	return GetModelRef(model);
}
 		 
void ColAndreasWorld::setMyExtraID(uint16_t index, int type, int data)
{
	objectManager->setExtraID(index, type, data);
}

int ColAndreasWorld::getMyExtraID(uint16_t index, int type)
{
	return objectManager->getExtraID(index, type);
}

bool ColAndreasWorld::loadCollisionData()
{
	if (LoadCollisionData(this->dynamicsWorld))
	{
		return true;
	}
	return false;
}
