////////////////////////////////////////////////////////////////////////////////
// ColAndreas Plugin Include ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#if defined _colandreas_included
	#endinput
#endif
#define _colandreas_included

#pragma library colandreas

#define COLANDREAS_VERSION (10500) //a.b.c 10000*a+100*b+c
#define COLANDREAS (COLANDREAS_VERSION)

// Default extra ID types
#define CA_EXTRA_1  0
#define CA_EXTRA_2  1
#define CA_EXTRA_3  2
#define CA_EXTRA_4  3
#define CA_EXTRA_5  4
#define CA_EXTRA_6  5
#define CA_EXTRA_7  6
#define CA_EXTRA_8  7
#define CA_EXTRA_9  8
#define CA_EXTRA_10 9

// ColAndreas currently supports 50000 user collision objects (This can be raised as needed)
#if defined MAX_CA_OBJECTS
    #if MAX_CA_OBJECTS > 50000
        #error [ColAndreas] MAX_CA_OBJECTS is too high, maximum value is 50000
    #endif
#else
   #define MAX_CA_OBJECTS 10000
#endif

// Water object ID
#define		WATER_OBJECT		20000

// Object types (Player objects are supported as dynamic only)
#define		OBJECT_TYPE_OBJECT	0
#define		OBJECT_TYPE_DYNAMIC	1

#if !defined FLOAT_INFINITY
	#define FLOAT_INFINITY (Float:0x7F800000)
#endif

// Maximum number of raycasts in CA_RayCastMultiLine
#define MAX_MULTICAST_SIZE 99

/**--------------------------------------------------------------------------**\
<summary>
	CA_Init
</summary>
<returns>
	0 when there is no data loaded (ex. when you don't have the data file)
	1 when loaded map successfully or was already loaded
</returns>
\**--------------------------------------------------------------------------**/
native CA_Init();

/**--------------------------------------------------------------------------**\
<summary>
	CA_RemoveBuilding
</summary>
<param name="modelid">The model to be removed</param>
<param name="Float:x, Float:y, Float:z">The coordinates in which the objects will be removed</param>
<param name="Float:radius">The radius around the specified point to remove from</param>
<returns>
	0 when the map is already initialized
	1 when successfully removed
</returns>
<remarks>
    You must use this function before using `CA_Init`
</remarks>
\**--------------------------------------------------------------------------**/
native CA_RemoveBuilding(modelid, Float:x, Float:y, Float:z, Float:radius);

/**--------------------------------------------------------------------------**\
<summary>
	CA_RestoreBuilding
</summary>
<param name="modelid">The removed model to be restored</param>
<param name="Float:x, Float:y, Float:z">The coordinates in which the objects will be restored</param>
<param name="Float:radius">The radius around the specified point to restore from</param>
<returns>
	0 when the map is not initialized
	1 when successfully restored
</returns>
<remarks>
    You must use this function after using `CA_Init`
</remarks>
\**--------------------------------------------------------------------------**/
native CA_RestoreBuilding(modelid, Float:x, Float:y, Float:z, Float:radius);

/**--------------------------------------------------------------------------**\
<summary>
	CA_RayCastLine
</summary>
<param name="Float:StartX, Float:StartY, Float:StartZ">The beginning point of the ray</param>
<param name="Float:EndX, Float:EndY, Float:EndZ">The ending point of the ray</param>
<param name="&Float:x, &Float:y, &Float:z">The point that the ray collided</param>
<returns>
	0 if the ray didn't collide with anything
    WATER_OBJECT if the ray collided with water
	The model of the object the ray collided
</returns>
\**--------------------------------------------------------------------------**/
native CA_RayCastLine(Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, &Float:x, &Float:y, &Float:z);

/**--------------------------------------------------------------------------**\
<summary>
	CA_RayCastLineID
</summary>
<param name="Float:StartX, Float:StartY, Float:StartZ">The beginning point of the ray</param>
<param name="Float:EndX, Float:EndY, Float:EndZ">The ending point of the ray</param>
<param name="&Float:x, &Float:y, &Float:z">The point that the ray collided</param>
<returns>
	-1 if the ray collided with a static object or water
	0 if the ray didn't collide with anything
	The index of the object the ray collided
</returns>
<remarks>
    This only works with objects created with `add` enabled in `CA_CreateObject`, such as the objects created by the "_DC" functions
</remarks>
\**--------------------------------------------------------------------------**/
native CA_RayCastLineID(Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, &Float:x, &Float:y, &Float:z);

/**--------------------------------------------------------------------------**\
<summary>
	CA_RayCastLineExtraID
</summary>
<param name="type">The extra ID index</param>
<param name="Float:StartX, Float:StartY, Float:StartZ">The beginning point of the ray</param>
<param name="Float:EndX, Float:EndY, Float:EndZ">The ending point of the ray</param>
<param name="&Float:x, &Float:y, &Float:z">The point that the ray collided</param>
<returns>
	-1 if the ray collided with a static object or water
    -1 if the extra ID is unmodified
	0 if the ray didn't collide with anything
	The index of the object the ray collided
</returns>
<remarks>
    This only works with objects created with `add` enabled in `CA_CreateObject`, such as the objects created by the "_DC" functions
</remarks>
\**--------------------------------------------------------------------------**/
native CA_RayCastLineExtraID(type, Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, &Float:x, &Float:y, &Float:z);

/**--------------------------------------------------------------------------**\
<summary>
	CA_RayCastMultiLine
</summary>
<param name="Float:StartX, Float:StartY, Float:StartZ">The beginning point of the ray</param>
<param name="Float:EndX, Float:EndY, Float:EndZ">The ending point of the ray</param>
<param name="Float:retx[], Float:rety[], Float:retz[]">The points that the ray collided</param>
<param name="Float:retdist[]">The distances between the beginning point and the point collided</param>
<param name="ModelIDs[]">The object models that the ray collided</param>
<param name="size = sizeof(retx)">The maximum number of points to collide</param>
<returns>
	-1 if the ray collided with more points than than permitted by the `size` parameter or `size` is higher than MAX_MULTICAST_SIZE value
	0 if the ray didn't collide with anything
	The number of points collided
</returns>
\**--------------------------------------------------------------------------**/
native CA_RayCastMultiLine(Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, Float:retx[], Float:rety[], Float:retz[], Float:retdist[], ModelIDs[], size = sizeof(retx));

/**--------------------------------------------------------------------------**\
<summary>
	CA_RayCastLineAngle
</summary>
<param name="Float:StartX, Float:StartY, Float:StartZ">The beginning point of the ray</param>
<param name="Float:EndX, Float:EndY, Float:EndZ">The ending point of the ray</param>
<param name="&Float:x, &Float:y, &Float:z">The point that the ray collided</param>
<param name="&Float:rx, &Float:ry, &Float:rz">The rotation of the face that the ray collided</param>
<returns>
	0 if the ray didn't collide with anything
    WATER_OBJECT if the ray collided with water
	The model of the object the ray collided
</returns>
\**--------------------------------------------------------------------------**/
native CA_RayCastLineAngle(Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, &Float:x, &Float:y, &Float:z, &Float:rx, &Float:ry, &Float:rz);

/**--------------------------------------------------------------------------**\
<summary>
	CA_RayCastReflectionVector
</summary>
<param name="Float:StartX, Float:StartY, Float:StartZ">The beginning point of the ray</param>
<param name="Float:EndX, Float:EndY, Float:EndZ">The ending point of the ray</param>
<param name="&Float:x, &Float:y, &Float:z">The point that the ray collided</param>
<param name="&Float:nx, &Float:ny, &Float:nz">The reflection vector of the face that the ray collided</param>
<returns>
	0 if the ray didn't collide with anything
    WATER_OBJECT if the ray collided with water
	The model of the object the ray collided
</returns>
\**--------------------------------------------------------------------------**/
native CA_RayCastReflectionVector(Float:startx, Float:starty, Float:startz, Float:endx, Float:endy, Float:endz, &Float:x, &Float:y, &Float:z, &Float:nx, &Float:ny, &Float:nz);

/**--------------------------------------------------------------------------**\
<summary>
	CA_RayCastLineNormal
</summary>
<param name="Float:StartX, Float:StartY, Float:StartZ">The beginning point of the ray</param>
<param name="Float:EndX, Float:EndY, Float:EndZ">The ending point of the ray</param>
<param name="&Float:x, &Float:y, &Float:z">The point that the ray collided</param>
<param name="&Float:nx, &Float:ny, &Float:nz">The surface normal of the face that the ray collided</param>
<returns>
	0 if the ray didn't collide with anything
    WATER_OBJECT if the ray collided with water
	The model of the object the ray collided
</returns>
\**--------------------------------------------------------------------------**/
native CA_RayCastLineNormal(Float:startx, Float:starty, Float:startz, Float:endx, Float:endy, Float:endz, &Float:x, &Float:y, &Float:z, &Float:nx, &Float:ny, &Float:nz);

/**--------------------------------------------------------------------------**\
<summary>
	CA_ContactTest
</summary>
<param name="modelid">The object model to be tested</param>
<param name="Float:x, Float:y, Float:z">The object position to test</param>
<param name="Float:rx, Float:ry, Float:rz">The object rotation to test</param>
<returns>
	0 if the object model doesn't collide with the world with the specified
    1 otherwise
</returns>
\**--------------------------------------------------------------------------**/
native CA_ContactTest(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);

/**--------------------------------------------------------------------------**\
<summary>
	CA_EulerToQuat
</summary>
<param name="Float:rx, Float:ry, Float:rz">GTA euler rotations to be converted</param>
<param name="&Float:x, &Float:y, &Float:z, &Float:w">The quaternion angles returned</param>
<returns>
    Always 1
</returns>
\**--------------------------------------------------------------------------**/
native CA_EulerToQuat(Float:rx, Float:ry, Float:rz, &Float:x, &Float:y, &Float:z, &Float:w);

/**--------------------------------------------------------------------------**\
<summary>
	CA_QuatToEuler
</summary>
<param name="&Float:x, &Float:y, &Float:z, &Float:w">The quaternion angles to be converted</param>
<param name="Float:rx, Float:ry, Float:rz">GTA euler rotations returned</param>
<returns>
    Always 1
</returns>
\**--------------------------------------------------------------------------**/
native CA_QuatToEuler(Float:x, Float:y, Float:z, Float:w, &Float:rx, &Float:ry, &Float:rz);

/**--------------------------------------------------------------------------**\
<summary>
	CA_GetModelBoundingSphere
</summary>
<param name="modelid">The object model</param>
<param name="&Float:offx, &Float:offy, &Float:offz">The offset of the model's bounding sphere</param>
<param name="&Float:radius">The radius of the model's bounding sphere</param>
<returns>
    0 if the model is invalid
    1 otherwise
</returns>
\**--------------------------------------------------------------------------**/
native CA_GetModelBoundingSphere(modelid, &Float:offx, &Float:offy, &Float:offz, &Float:radius);

/**--------------------------------------------------------------------------**\
<summary>
	CA_GetModelBoundingBox
</summary>
<param name="modelid">The object model</param>
<param name="&Float:minx, &Float:miny, &Float:minz">The "minimum" point of the model's bounding box</param>
<param name="&Float:maxx, &Float:maxy, &Float:maxz">The "maximum" point of the model's bounding box</param>
<returns>
    0 if the model is invalid
    1 otherwise
</returns>
\**--------------------------------------------------------------------------**/
native CA_GetModelBoundingBox(modelid, &Float:minx, &Float:miny, &Float:minz, &Float:maxx, &Float:maxy, &Float:maxz);

/**--------------------------------------------------------------------------**\
<summary>
	CA_SetObjectExtraID
</summary>
<param name="index">The index of the ColAndreas object</param>
<param name="type">The index of the extra ID to be set</param>
<param name="data">The data stored in the extra ID</param>
<returns>
    Always 1 (which needs to be changed to be like CA_GetObjectExtraID's returns...)
</returns>
\**--------------------------------------------------------------------------**/
native CA_SetObjectExtraID(index, type, data);

/**--------------------------------------------------------------------------**\
<summary>
	CA_GetObjectExtraID
</summary>
<param name="index">The index of the ColAndreas object</param>
<param name="type">The index of the extra ID to be set</param>
<returns>
    -1 if the index or type is invalid
    The data stored in the extra ID
</returns>
\**--------------------------------------------------------------------------**/
native CA_GetObjectExtraID(index, type);

/**--------------------------------------------------------------------------**\
<summary>
	CA_RayCastLineEx
</summary>
<param name="Float:StartX, Float:StartY, Float:StartZ">The beginning point of the ray</param>
<param name="Float:EndX, Float:EndY, Float:EndZ">The ending point of the ray</param>
<param name="&Float:x, &Float:y, &Float:z">The point that the ray collided</param>
<param name="&Float:rx, &Float:ry, &Float:rz, &Float:rw">The quaternion rotation of the object that the ray collided</param>
<param name="&Float:cx, &Float:cy, &Float:cz">The position of the object that the ray collided</param>
<returns>
	0 if the ray didn't collide with anything
    WATER_OBJECT if the ray collided with water
	The model of the object the ray collided
</returns>
\**--------------------------------------------------------------------------**/
native CA_RayCastLineEx(Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, &Float:x, &Float:y, &Float:z, &Float:rx, &Float:ry, &Float:rz, &Float:rw, &Float:cx, &Float:cy, &Float:cz);

/**--------------------------------------------------------------------------**\
<summary>
	CA_RayCastLineAngleEx
</summary>
<param name="Float:StartX, Float:StartY, Float:StartZ">The beginning point of the ray</param>
<param name="Float:EndX, Float:EndY, Float:EndZ">The ending point of the ray</param>
<param name="&Float:x, &Float:y, &Float:z">The point that the ray collided</param>
<param name="&Float:rx, &Float:ry, &Float:rz">The rotation of the face that the ray collided</param>
<param name="&Float:ocx, &Float:ocy, &Float:ocz">The position of the object that the ray collided</param>
<param name="&Float:orx, &Float:ory, &Float:orz">The rotation of the object that the ray collided</param>
<returns>
	0 if the ray didn't collide with anything
    WATER_OBJECT if the ray collided with water
	The model of the object the ray collided
</returns>
\**--------------------------------------------------------------------------**/
native CA_RayCastLineAngleEx(Float:StartX, Float:StartY, Float:StartZ, Float:EndX, Float:EndY, Float:EndZ, &Float:x, &Float:y, &Float:z, &Float:rx, &Float:ry, &Float:rz, &Float:ocx, &Float:ocy, &Float:ocz, &Float:orx, &Float:ory, &Float:orz);

// Internal Functions:

/**--------------------------------------------------------------------------**\
<summary>
	CA_LoadFromDff
</summary>
<param name="newid">Custom model id to be added to col. pool</param>
<param name="dffFileName">Dff file path you want to load its collision</param>
<returns>
	-1 if plugin couldn't find/open file (relatd to permissions or wrong path)
	0 if dff file had no collision or corrupted data
	1 if loading dff file and reading collision data was successful
</returns>
<remarks>
	Loads collision from given dff file to support SA-MP 0.3DL custom models.
</remarks>
\**--------------------------------------------------------------------------**/
native CA_LoadFromDff(newid, const dffFileName[]);

/**--------------------------------------------------------------------------**\
<summary>
	CA_CreateObject
</summary>
<param name="modelid">The collision model to create</param>
<param name="Float:x, Float:y, Float:z">The position in which to create the collision model</param>
<param name="Float:rx, Float:ry, Float:rz">The rotation of the collision model</param>
<param name="bool:add = false">Whether or not to store this objects index for manual management</param>
<returns>
	-1 if the specified model has no collision (should be changed to -2, to avoid confusion with unmanaged objects)
    -1 if `add` is false
	The index of the created collision object
</returns>
<remarks>
	ONLY CREATES THE COLLISION, NOT THE IN-GAME OBJECT
</remarks>
\**--------------------------------------------------------------------------**/
native CA_CreateObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, bool:add = false);

/**--------------------------------------------------------------------------**\
<summary>
	CA_DestroyObject
</summary>
<param name="index">The index of the object to be destroyed</param>
<returns>
	-1 if the specified model has no collision (should be changed to -2, to avoid confusion with unmanaged objects)
    -1 if `add` is false
	The index of the created collision object
</returns>
\**--------------------------------------------------------------------------**/
native CA_DestroyObject(index);

/**--------------------------------------------------------------------------**\
<summary>
	CA_IsValidObject
</summary>
<param name="index">Checks if an object with the ID provided exists.</param>
<returns>
	-1 if the specified model has no collision
	0 if the object does not exist
	1 if the object exists
</returns>
\**--------------------------------------------------------------------------**/
native CA_IsValidObject(index);

/**--------------------------------------------------------------------------**\
<summary>
	CA_SetObjectPos
</summary>
<param name="index">The index of the object to be moved</param>
<param name="Float:x, Float:y, Float:z">The new position</param>
<returns>
	0 if the object doesn't exist
	1 if the new position was set successfully
</returns>
\**--------------------------------------------------------------------------**/
native CA_SetObjectPos(index, Float:x, Float:y, Float:z);

/**--------------------------------------------------------------------------**\
<summary>
	CA_SetObjectRot
</summary>
<param name="index">The index of the object to be moved</param>
<param name="Float:rx, Float:ry, Float:rz">The new rotation</param>
<returns>
	0 if the object doesn't exist
	1 if the new rotation was set successfully
</returns>
\**--------------------------------------------------------------------------**/
native CA_SetObjectRot(index, Float:rx, Float:ry, Float:rz);

#if defined _Y_ITERATE_LOCAL_VERSION
	static stock Iterator:CA_Objects<MAX_CA_OBJECTS>;
#endif

enum CAOBJECTINFO
{
	ColdAndreadsID,
	ObjectID,
	ObjectType,

	#if !defined _Y_ITERATE_LOCAL_VERSION
		bool:ObjectUsed,
	#endif
}

static stock CA_ObjectList[MAX_CA_OBJECTS][CAOBJECTINFO];

//Streamer
#if defined STREAMER_TYPE_OBJECT
	// Static collision object functions (The index is not returned these can not be deleted!)
	stock STREAMER_TAG_OBJECT:CA_CreateDynamicObjectEx_SC(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:streamdistance = STREAMER_OBJECT_SD, Float:drawdistance = STREAMER_OBJECT_DD, worlds[] = { -1 }, interiors[] = { -1 }, players[] = { -1 }, STREAMER_TAG_AREA:areas[] = { STREAMER_TAG_AREA:-1 }, priority = 0, maxworlds = sizeof worlds, maxinteriors = sizeof interiors, maxplayers = sizeof players, maxareas = sizeof areas)
	{
		new STREAMER_TAG_OBJECT:id = CreateDynamicObjectEx(modelid, x, y, z, rx, ry, rz, streamdistance, drawdistance, worlds, interiors, players, areas, priority, maxworlds, maxinteriors, maxplayers, maxareas);
		CA_CreateObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
		return id;
	}

	stock STREAMER_TAG_OBJECT:CA_CreateDynamicObject_SC(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, vw = -1, interior = -1, playerid = -1, Float:streamdistance = STREAMER_OBJECT_SD, Float:drawdistance = STREAMER_OBJECT_DD, STREAMER_TAG_AREA:areaid = STREAMER_TAG_AREA:-1, priority = 0)
	{
		new STREAMER_TAG_OBJECT:id = CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, vw, interior, playerid, streamdistance, drawdistance, areaid, priority);
		CA_CreateObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
		return id;
	}

	stock CA_CreateObject_SC(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:drawdistance = 300.0)
	{
		new id = CreateObject(modelid, x, y, z, rx, ry, rz, drawdistance);
		if(id != INVALID_OBJECT_ID) CA_CreateObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
		return id;
	}

	// Dynamic collision object functions (Removable)
	stock CA_CreateDynamicObjectEx_DC(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:streamdistance = STREAMER_OBJECT_SD, Float:drawdistance = STREAMER_OBJECT_DD, worlds[] = { -1 }, interiors[] = { -1 }, players[] = { -1 }, STREAMER_TAG_AREA:areas[] = { STREAMER_TAG_AREA:-1 }, priority = 0, maxworlds = sizeof worlds, maxinteriors = sizeof interiors, maxplayers = sizeof players, maxareas = sizeof areas)
	{
		new index = -1;
		#if defined _Y_ITERATE_LOCAL_VERSION
			index = Iter_Free(CA_Objects);
		#else
			for(new i = 0; i < MAX_CA_OBJECTS; i++)
			{
				if(CA_ObjectList[i][ObjectUsed] == false)
				{
					index = i;
					break;
				}
			}
		#endif

		if(index > -1)
		{
			#if defined _Y_ITERATE_LOCAL_VERSION
				Iter_Add(CA_Objects, index);
			#else
				CA_ObjectList[index][ObjectUsed] = true;
			#endif
			CA_ObjectList[index][ObjectID] = _:CreateDynamicObjectEx(modelid, x, y, z, rx, ry, rz, drawdistance, streamdistance, worlds, interiors, players, areas, priority, maxworlds, maxinteriors, maxplayers, maxareas);
			CA_ObjectList[index][ColdAndreadsID] = CA_CreateObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, true);
			CA_ObjectList[index][ObjectType] = OBJECT_TYPE_DYNAMIC;
		}
		return index;
	}


	stock CA_CreateDynamicObject_DC(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, vw = -1, interior = -1, playerid = -1, Float:streamdistance = STREAMER_OBJECT_SD, Float:drawdistance = STREAMER_OBJECT_DD, STREAMER_TAG_AREA:areaid = STREAMER_TAG_AREA:-1, priority = 0)
	{
		new index = -1;
		#if defined _Y_ITERATE_LOCAL_VERSION
			index = Iter_Free(CA_Objects);
		#else
			for(new i = 0; i < MAX_CA_OBJECTS; i++)
			{
				if(CA_ObjectList[i][ObjectUsed] == false)
				{
					index = i;
					break;
				}
			}
		#endif

		if(index > -1)
		{
			#if defined _Y_ITERATE_LOCAL_VERSION
				Iter_Add(CA_Objects, index);
			#else
				CA_ObjectList[index][ObjectUsed] = true;
			#endif
			CA_ObjectList[index][ObjectID] = _:CreateDynamicObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, vw, interior, playerid, streamdistance, drawdistance, areaid, priority);
			CA_ObjectList[index][ColdAndreadsID] = CA_CreateObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, true);
			CA_ObjectList[index][ObjectType] = OBJECT_TYPE_DYNAMIC;
		}
		return index;
	}
#endif

/**--------------------------------------------------------------------------**\
<summary>
	CA_CreateObject_DC
</summary>
<param name="modelid">The model to create</param>
<param name="Float:x, Float:y, Float:z">The position in which to create the object</param>
<param name="Float:rx, Float:ry, Float:rz">The rotation of the object</param>
<param name="Float:drawdistance = 300.0">Whether or not to store this objects index for manual management</param>
<returns>
    -1 if the object couldn't be created
    -1 if there are too many "DC" objects
	The "DC" index of the created object
</returns>
<remarks>
    The index returned by this function can only be used by "DC" functions
</remarks>
\**--------------------------------------------------------------------------**/
stock CA_CreateObject_DC(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:drawdistance = 300.0)
{
	new index = -1;
	#if defined _Y_ITERATE_LOCAL_VERSION
		index = Iter_Free(CA_Objects);
	#else
		for(new i = 0; i < MAX_CA_OBJECTS; i++)
		{
			if(CA_ObjectList[i][ObjectUsed] == false)
			{
				index = i;
				break;
			}
		}
	#endif

	new id = CreateObject(modelid, x, y, z, rx, ry, rz, drawdistance);
	if(id == INVALID_OBJECT_ID) return -1;
	if(index > -1)
	{
		#if defined _Y_ITERATE_LOCAL_VERSION
			Iter_Add(CA_Objects, index);
		#else
			CA_ObjectList[index][ObjectUsed] = true;
		#endif
		CA_ObjectList[index][ObjectID] = id;
		CA_ObjectList[index][ColdAndreadsID] = CA_CreateObject(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, true);
		CA_ObjectList[index][ObjectType] = OBJECT_TYPE_OBJECT;
	}
	return index;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_DestroyObject_DC
</summary>
<param name="index">The DC index of the object to destroy</param>
<returns>
    -1 if the index is invalid
    -1 if the object doesn't exist
    1 or next interator index if the object was destroyed
</returns>
\**--------------------------------------------------------------------------**/
stock CA_DestroyObject_DC(index)
{
	// Out of bounds
	if(index < 0 || index >= MAX_CA_OBJECTS) return -1;
	#if defined _Y_ITERATE_LOCAL_VERSION
		if(Iter_Contains(CA_Objects, index))
		{
			new next;
			Iter_SafeRemove(CA_Objects, index, next);
			if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_OBJECT) DestroyObject(CA_ObjectList[index][ObjectID]);
			#if defined STREAMER_TYPE_OBJECT
			else if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_DYNAMIC) DestroyDynamicObject(STREAMER_TAG_OBJECT:CA_ObjectList[index][ObjectID]);
			#endif
			CA_DestroyObject(CA_ObjectList[index][ColdAndreadsID]);
			return next;
		}
	#else
	if(CA_ObjectList[index][ObjectUsed])
	{
		if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_OBJECT) DestroyObject(CA_ObjectList[index][ObjectID]);
		#if defined STREAMER_TYPE_OBJECT
		else if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_DYNAMIC) DestroyDynamicObject(STREAMER_TAG_OBJECT:CA_ObjectList[index][ObjectID]);
		#endif
		CA_ObjectList[index][ObjectUsed] = false;
		CA_DestroyObject(CA_ObjectList[index][ColdAndreadsID]);
		return 1;
	}
	#endif
	return -1;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_SetObjectPos_DC
</summary>
<param name="index">The DC index of the object to be moved</param>
<param name="Float:x, Float:y, Float:z">The new position</param>
<returns>
    -1 if the index is invalid
    -1 if the object doesn't exist
	1 if the new position was set successfully
</returns>
\**--------------------------------------------------------------------------**/
stock CA_SetObjectPos_DC(index, Float:x, Float:y, Float:z)
{
	// Out of bounds
	if(index < 0 || index >= MAX_CA_OBJECTS) return -1;
	#if defined _Y_ITERATE_LOCAL_VERSION
		if(Iter_Contains(CA_Objects, index))
		{
			if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_OBJECT) SetObjectPos(CA_ObjectList[index][ObjectID], x, y, z);
			#if defined STREAMER_TYPE_OBJECT
				else if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_DYNAMIC) SetDynamicObjectPos(STREAMER_TAG_OBJECT:CA_ObjectList[index][ObjectID], x, y, z);
			#endif
			CA_SetObjectPos(CA_ObjectList[index][ColdAndreadsID], x, y, z);
		}
	#else
	if(CA_ObjectList[index][ObjectUsed])
	{
		if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_OBJECT) SetObjectPos(CA_ObjectList[index][ObjectID], x, y, z);
		#if defined STREAMER_TYPE_OBJECT
		else if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_DYNAMIC) SetDynamicObjectPos(STREAMER_TAG_OBJECT:CA_ObjectList[index][ObjectID], x, y, z);
		#endif
		SetObjectPos(CA_ObjectList[index][ObjectID], x, y, z);
		CA_SetObjectPos(CA_ObjectList[index][ColdAndreadsID], x, y, z);
		return 1;
	}
	#endif
	return -1;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_SetObjectRot_DC
</summary>
<param name="index">The DC index of the object to be moved</param>
<param name="Float:rx, Float:ry, Float:rz">The new rotation</param>
<returns>
    -1 if the index is invalid
    -1 if the object doesn't exist
	1 if the new rotation was set successfully
</returns>
\**--------------------------------------------------------------------------**/
stock CA_SetObjectRot_DC(index, Float:rx, Float:ry, Float:rz)
{
	// Out of bounds
	if(index < 0 || index >= MAX_CA_OBJECTS) return -1;
	#if defined _Y_ITERATE_LOCAL_VERSION
		if(Iter_Contains(CA_Objects, index))
		{
			if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_OBJECT) SetObjectRot(CA_ObjectList[index][ObjectID], rx, ry, rz);
			#if defined STREAMER_TYPE_OBJECT
				else if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_DYNAMIC) SetDynamicObjectRot(STREAMER_TAG_OBJECT:CA_ObjectList[index][ObjectID], rx, ry, rz);
			#endif
			CA_SetObjectRot(CA_ObjectList[index][ColdAndreadsID], rx, ry, rz);
		}
	#else
	if(CA_ObjectList[index][ObjectUsed])
	{
		if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_OBJECT) SetObjectRot(CA_ObjectList[index][ObjectID], rx, ry, rz);
		#if defined STREAMER_TYPE_OBJECT
			else if(CA_ObjectList[index][ObjectType] == OBJECT_TYPE_DYNAMIC) SetDynamicObjectRot(STREAMER_TAG_OBJECT:CA_ObjectList[index][ObjectID], rx, ry, rz);
		#endif
		CA_SetObjectRot(CA_ObjectList[index][ColdAndreadsID], rx, ry, rz);
		return 1;
	}
	#endif
	return -1;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_DestroyAllObjects_DC
</summary>
<remarks>
    Destroys all DC objects
</remarks>
\**--------------------------------------------------------------------------**/
stock CA_DestroyAllObjects_DC()
{
	#if defined _Y_ITERATE_LOCAL_VERSION
		foreach(new i : CA_Objects)
		{
			if(CA_ObjectList[i][ObjectType] == OBJECT_TYPE_OBJECT) i = CA_DestroyObject_DC(i);
		}
	#else
		for(new i = 0; i < MAX_CA_OBJECTS; i++)
		{
			if(CA_ObjectList[i][ObjectUsed])
			{
				if(CA_ObjectList[i][ObjectType] == OBJECT_TYPE_OBJECT) CA_DestroyObject_DC(i);
			}
		}

	#endif
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_FindZ_For2DCoord
</summary>
<remarks>
    The ColAndreas alternative for MapAndreas's entire function
    This function does everything MapAndreas is capable of doing, and without the ground limit
</remarks>
\**--------------------------------------------------------------------------**/
stock CA_FindZ_For2DCoord(Float:x, Float:y, &Float:z)
{
	if(CA_RayCastLine(x, y, 700.0, x, y, -1000.0, x, y, z)) return 1;
	return 0;
}

// Explode ray casts in every direction
stock CA_RayCastExplode(Float:cX, Float:cY, Float:cZ, Float:Radius, Float:intensity = 20.0, Float:collisions[][3])
{
	if(intensity < 1.0 || intensity > 360.0 || (((360.0 / intensity) - floatround((360.0 / intensity), floatround_floor)) * intensity))
		return 0;

	new index, Float:LAT, Float:LON, Float:x, Float:y, Float:z;
	for(new Float:lat = -180.0, Float:lon = -90.0; lat < 180.0; lat += (intensity * 0.75))
	{
		for(lon = -90.0; lon < 90.0; lon += intensity)
		{
			LAT = lat * 3.141593 / 180.0;
			LON = lon * 3.141593 / 180.0;
			x = -Radius * floatcos(LAT) * floatcos(LON);
			y = Radius * floatcos(LAT) * floatsin(LON);
			z = Radius * floatsin(LAT);

			if(CA_RayCastLine(cX, cY, cZ, cX + x, cY + y, cZ + z, x, y, z))
			{
				collisions[index][0] = x;
				collisions[index][1] = y;
				collisions[index][2] = z;
				index++;
			}
		}
	}

	return index;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_IsPlayerOnSurface
</summary>
<param name="playerid">The playerid to check</param>
<param name="Float:tolerance = 1.5">The distance to check for the ground</param>
<returns>
    0 if the player is not on the ground
    1 if the player is on the ground
</returns>
\**--------------------------------------------------------------------------**/
stock CA_IsPlayerOnSurface(playerid, Float:tolerance=1.5)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	// Check if player is actually on the ground
	if(!CA_RayCastLine(x, y, z, x, y, z-tolerance, x, y, z))
		return 0;
	return 1;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_RemoveBarriers
</summary>
<returns>
	Always 1
</returns>
<remarks>
    Removes all of the barrier object collisions
    You must use this function before using `CA_Init`
</remarks>
\**--------------------------------------------------------------------------**/
stock CA_RemoveBarriers()
{
	static const BarrierIDS[] = {
		4504, 4505, 4506, 4507, 4508, 4509, 4510, 4511, 4512, 4513,
		4514, 4515, 4516, 4517, 4518, 4519, 4520, 4521, 4522, 4523,
		4524, 4525, 4526, 4527, 16436, 16437, 16438, 16439, 1662
 	};

 	for(new i = 0; i < sizeof(BarrierIDS); i++)
		CA_RemoveBuilding(BarrierIDS[i], 0.0, 0.0, 0.0, 4242.6407);

	return 1;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_RemoveBreakableBuildings
</summary>
<returns>
	Always 1
</returns>
<remarks>
    Removes all of the breakable object collisions
    You must use this function before using `CA_Init`
</remarks>
\**--------------------------------------------------------------------------**/
stock CA_RemoveBreakableBuildings()
{
	static const BreakableIDs[] = {
		625, 626, 627, 628, 629, 630, 631, 632, 633, 642, 643, 644, 646, 650, 716, 717, 737, 738, 792, 858, 881, 882, 883,
		884, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894, 895, 904, 905, 941, 955, 956, 959, 961, 990, 993, 996, 1209,
		1211, 1213, 1219, 1220, 1221, 1223, 1224, 1225, 1226, 1227, 1228, 1229, 1230, 1231, 1232, 1235, 1238, 1244, 1251,
		1255, 1257, 1262, 1264, 1265, 1270, 1280, 1281, 1282, 1283, 1284, 1285, 1286, 1287, 1288, 1289, 1290, 1291, 1293,
		1294, 1297, 1300, 1302, 1315, 1328, 1329, 1330, 1338, 1350, 1351, 1352, 1370, 1373, 1374, 1375, 1407, 1408, 1409,
		1410, 1411, 1412, 1413, 1414, 1415, 1417, 1418, 1419, 1420, 1421, 1422, 1423, 1424, 1425, 1426, 1428, 1429, 1431,
		1432, 1433, 1436, 1437, 1438, 1440, 1441, 1443, 1444, 1445, 1446, 1447, 1448, 1449, 1450, 1451, 1452, 1456, 1457,
		1458, 1459, 1460, 1461, 1462, 1463, 1464, 1465, 1466, 1467, 1468, 1469, 1470, 1471, 1472, 1473, 1474, 1475, 1476,
		1477, 1478, 1479, 1480, 1481, 1482, 1483, 1514, 1517, 1520, 1534, 1543, 1544, 1545, 1551, 1553, 1554, 1558, 1564,
		1568, 1582, 1583, 1584, 1588, 1589, 1590, 1591, 1592, 1645, 1646, 1647, 1654, 1664, 1666, 1667, 1668, 1669, 1670,
		1672, 1676, 1684, 1686, 1775, 1776, 1949, 1950, 1951, 1960, 1961, 1962, 1975, 1976, 1977, 2647, 2663, 2682, 2683,
		2885, 2886, 2887, 2900, 2918, 2920, 2925, 2932, 2933, 2942, 2943, 2945, 2947, 2958, 2959, 2966, 2968, 2971, 2977,
		2987, 2988, 2989, 2991, 2994, 3006, 3018, 3019, 3020, 3021, 3022, 3023, 3024, 3029, 3032, 3036, 3058, 3059, 3067,
		3083, 3091, 3221, 3260, 3261, 3262, 3263, 3264, 3265, 3267, 3275, 3276, 3278, 3280, 3281, 3282, 3302, 3374, 3409,
		3460, 3516, 3794, 3795, 3797, 3853, 3855, 3864, 3884, 11103, 12840, 16627, 16628, 16629, 16630, 16631, 16632,
		16633, 16634, 16635, 16636, 16732, 17968
	};

 	for(new i = 0; i < sizeof(BreakableIDs); i++)
		CA_RemoveBuilding(BreakableIDs[i], 0.0, 0.0, 0.0, 4242.6407);

	return 1;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_IsPlayerInWater
</summary>
<param name="playerid">The playerid to check</param>
<param name="&Float:depth">The depth</param>
<param name="&Float:playerdepth">The depth of the player</param>
<returns>
    0 if the player is not in water
    1 if the player is in water
</returns>
\**--------------------------------------------------------------------------**/
stock CA_IsPlayerInWater(playerid, &Float:depth, &Float:playerdepth)
{
	new Float:x, Float:y, Float:z, Float:retx[10], Float:rety[10], Float:retz[10], Float: retdist[10], modelids[10];
	GetPlayerPos(playerid, x, y, z);
	new collisions = CA_RayCastMultiLine(x, y, z+1000.0, x, y, z-1000.0, retx, rety, retz, retdist, modelids, 10);
	if(collisions > 0)
	{
		for(new i = 0, j = 0; i < collisions; i++)
		{
			if(modelids[i] == WATER_OBJECT)
			{
				depth = FLOAT_INFINITY;

				for(j = 0; j < collisions; j++)
				{
					if(retz[j] < depth)
					depth = retz[j];
				}

				depth = retz[i] - depth;
				if(depth < 0.001 && depth > -0.001)
					depth = 100.0;
				playerdepth = retz[i] - z;

				if(playerdepth < -2.0)
					return 0;

				return 1;
			}
		}
	}
	return 0;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_IsVehicleInWater
</summary>
<param name="vehicleid">The vehicleid to check</param>
<param name="&Float:depth">The depth</param>
<param name="&Float:vehicledepth">The depth of the vehicle</param>
<returns>
    0 if the vehicle is not in water
    1 if the vehicle is in water
</returns>
\**--------------------------------------------------------------------------**/
stock CA_IsVehicleInWater(vehicleid, &Float:depth, &Float:vehicledepth)
{
	new Float:x, Float:y, Float:z, Float:retx[10], Float:rety[10], Float:retz[10], Float:retdist[10], modelids[10];
	GetVehiclePos(vehicleid, x, y, z);
	new collisions = CA_RayCastMultiLine(x, y, z + 1000.0, x, y, z - 1000.0, retx, rety, retz, retdist, modelids, 10);
	if(collisions)
	{
		for(new i = 0, j = 0; i < collisions; i++)
		{
			if(modelids[i] == WATER_OBJECT)
			{
				depth = FLOAT_INFINITY;

				for(j = 0; j < collisions; j++)
				{
					if(retz[j] < depth)
					depth = retz[j];
				}

				depth = retz[i] - depth;
				if(depth < 0.001 && depth > -0.001)
					depth = 100.0;
				vehicledepth = retz[i] - z;

				if(vehicledepth < -2.0)
					return 0;

				return 1;
			}
		}
	}
	return 0;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_IsPlayerNearWater
</summary>
<param name="playerid">The playerid to check</param>
<param name="Float:dist = 3.0">The distance to check for water</param>
<param name="Float:height = 3.0">The height the player can be from the water</param>
<returns>
    0 if the player is not in water
    1 if the player is in water
</returns>
<remarks>
    Checks for water all around the player
</remarks>
\**--------------------------------------------------------------------------**/
stock CA_IsPlayerNearWater(playerid, Float:dist=3.0, Float:height=3.0)
{
	new Float:x, Float:y, Float:z, Float:tmp;
	GetPlayerPos(playerid, x, y, z);

	for(new i; i < 6; i++)
		if(CA_RayCastLine(x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z + height, x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z - height, tmp, tmp, tmp) == WATER_OBJECT)
			return 1;
	return 0;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_IsVehicleNearWater
</summary>
<param name="vehicleid">The vehicleid to check</param>
<param name="Float:dist = 3.0">The distance to check for water</param>
<param name="Float:height = 3.0">The height the vehicle can be from the water</param>
<returns>
    0 if the vehicle is not in water
    1 if the vehicle is in water
</returns>
<remarks>
    Checks for water all around the vehicle
</remarks>
\**--------------------------------------------------------------------------**/
stock CA_IsVehicleNearWater(vehicleid, Float:dist=3.0, Float:height=3.0)
{
	new Float:x, Float:y, Float:z, Float:tmp;
	GetVehiclePos(vehicleid, x, y, z);

	for(new i; i < 6; i++)
		if(CA_RayCastLine(x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z + height, x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z - height, tmp, tmp, tmp) == WATER_OBJECT)
			return 1;
	return 0;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_IsPlayerFacingWater
</summary>
<param name="playerid">The playerid to check</param>
<param name="Float:dist = 3.0">The distance to check for water</param>
<param name="Float:height = 3.0">The height the player can be from the water</param>
<returns>
    0 if the player is not in water
    1 if the player is in water
</returns>
<remarks>
    Checks for water only in front of the player
</remarks>
\**--------------------------------------------------------------------------**/
stock CA_IsPlayerFacingWater(playerid, Float:dist=3.0, Float:height=3.0)
{
	new Float:x, Float:y, Float:z, Float:r, Float:tmp;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, r);

	if(CA_RayCastLine(x + (dist * floatsin(-r, degrees)), y + (dist * floatcos(-r, degrees)), z, x + (dist * floatsin(-r, degrees)), y + (dist * floatcos(-r, degrees)), z - height, tmp, tmp, tmp) == WATER_OBJECT)
		return 1;
	return 0;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_IsVehicleFacingWater
</summary>
<param name="vehicleid">The vehicleid to check</param>
<param name="Float:dist = 3.0">The distance to check for water</param>
<param name="Float:height = 3.0">The height the vehicle can be from the water</param>
<returns>
    0 if the vehicle is not in water
    1 if the vehicle is in water
</returns>
<remarks>
    Checks for water only in front of the vehicle
</remarks>
\**--------------------------------------------------------------------------**/
stock CA_IsVehicleFacingWater(vehicleid, Float:dist=3.0, Float:height=3.0)
{
	new Float:x, Float:y, Float:z, Float:r, Float:tmp;
	GetVehiclePos(vehicleid, x, y, z);
	GetVehicleZAngle(vehicleid, r);

	if(CA_RayCastLine(x + (dist * floatsin(-r, degrees)), y + (dist * floatcos(-r, degrees)), z, x + (dist * floatsin(-r, degrees)), y + (dist * floatcos(-r, degrees)), z - height, tmp, tmp, tmp) == WATER_OBJECT)
		return 1;
	return 0;
}

/**--------------------------------------------------------------------------**\
<summary>
	CA_IsPlayerBlocked
</summary>
<param name="playerid">The playerid to check</param>
<param name="Float:dist = 1.5">The distance to check for a wall</param>
<param name="Float:height = 0.5">The height the wall can be from the ground</param>
<returns>
    0 if the player is not blocked
    1 if the player is blocked
</returns>
\**--------------------------------------------------------------------------**/
stock CA_IsPlayerBlocked(playerid, Float:dist=1.5, Float:height=0.5)
{
	new Float:x, Float:y, Float:z, Float:endx, Float:endy, Float:fa;
	GetPlayerPos(playerid, x, y, z);
	z -= 1.0 + height;
	GetPlayerFacingAngle(playerid, fa);

	endx = (x + dist * floatsin(-fa,degrees));
	endy = (y + dist * floatcos(-fa,degrees));
	if(CA_RayCastLine(x, y, z, endx, endy, z, x, y, z))
		return 1;
	return 0;
}

/**--------------------------------------------------------------------------**\
<summary>
	Float:CA_GetRoomHeight
</summary>
<param name="Float:x, Float:y, Float:z">A point in the area to be checked</param>
<returns>
    0.0 if there is either no floor or no ceiling
    The distance from the ceiling to the floor
</returns>
\**--------------------------------------------------------------------------**/
stock Float:CA_GetRoomHeight(Float:x, Float:y, Float:z)
{
	new Float:fx, Float:fy, Float:fz, Float:cx, Float:cy, Float:cz;
	if(CA_RayCastLine(x, y, z, x, y, z-1000.0, fx, fy, fz))
	{
		if(CA_RayCastLine(x, y, z, x, y, z+1000.0, cx, cy, cz))
			return floatsqroot(((fx-cx)*(fx-cx))+((fy-cy)*(fy-cy))+((fz-cz)*(fz-cz)));
	}
	return 0.0;
}

/**--------------------------------------------------------------------------**\
<summary>
	Float:CA_GetRoomCenter
</summary>
<param name="Float:x, Float:y, Float:z">A point in the area to be checked</param>
<param name="&Float:m_x, &Float:m_y">The center X and Y of the area</param>
<returns>
    -1.0 if there isn't enough collisions to determine the center
    The radius of the circle formed by the test
</returns>
\**--------------------------------------------------------------------------**/
stock Float:CA_GetRoomCenter(Float:x, Float:y, Float:z, &Float:m_x, &Float:m_y)
{
	new Float:pt1x, Float:pt1y,
		Float:pt2x, Float:pt2y,
		Float:pt3x, Float:pt3y,
		Float:tmp;

	if(!CA_RayCastLine(x, y, z, x + 1000.0 * floatcos(0.0, degrees), y + 1000.0 * floatsin(0.0, degrees), z, pt1x, pt1y, tmp) ||
		!CA_RayCastLine(x, y, z, x + 1000.0 * floatcos(120.0, degrees), y + 1000.0 * floatsin(120.0, degrees), z, pt2x, pt2y, tmp) ||
		!CA_RayCastLine(x, y, z, x + 1000.0 * floatcos(-120.0, degrees), y + 1000.0 * floatsin(-120.0, degrees), z, pt3x, pt3y, tmp))
		return -1.0;

	new Float:yDelta_a = pt2y - pt1y,
		Float:xDelta_a = pt2x - pt1x,
		Float:yDelta_b = pt3y - pt2y,
		Float:xDelta_b = pt3x - pt2x;

	if (floatabs(xDelta_a) <= 0.000000001 && floatabs(yDelta_b) <= 0.000000001) {
		m_x = 0.5 * (pt2x + pt3x);
		m_y = 0.5 * (pt1y + pt2y);
		return VectorSize(m_x - pt1x, m_y - pt1y, 0.0);
	}

	new Float:aSlope = yDelta_a / xDelta_a,
		Float:bSlope = yDelta_b / xDelta_b;

	if (floatabs(aSlope-bSlope) <= 0.000000001)
		return -1.0;

	m_x = (aSlope * bSlope * (pt1y - pt3y) + bSlope * (pt1x + pt2x) - aSlope * (pt2x + pt3x)) / (2.0 * (bSlope - aSlope));
	m_y = -1.0 * (m_x - (pt1x + pt2x) / 2.0) / aSlope + (pt1y + pt2y) / 2.0;

	return VectorSize(m_x - pt1x, m_y - pt1y, 0.0);
}

// Hooked functions
public OnFilterScriptExit()
{
	CA_DestroyAllObjects_DC();
	if (funcidx("CA_OnFilterScriptExit") != -1)
	{
		return CallLocalFunction("CA_OnFilterScriptExit", "");
	}
	return 1;
 }

#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
#define OnFilterScriptExit CA_OnFilterScriptExit

forward CA_OnFilterScriptExit();

public OnGameModeExit()
{
	CA_DestroyAllObjects_DC();
	if (funcidx("CA_OnGameModeExit") != -1)
	{
		return CallLocalFunction("CA_OnGameModeExit", "");
	}
	return 1;
 }

#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit CA_OnGameModeExit

forward CA_OnGameModeExit();

/*
Create an object with collision in ColAndreas dynamic
CA_CreateObject_DC(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:drawdistance = 300.0)

Destroy an object with collision in ColAndreas
CA_DestroyObject_DC(index)

Destoys all objects streamer/regular
CA_DestroyAllObjects_DC()

MapAndreas replacement function
CA_FindZ_For2DCoord(Float:x, Float:y, &Float:z)

Explode ray casts in every direction
CA_RayCastExplode(Float:cX, Float:cY, Float:cZ, Float:Radius, Float:intensity = 20.0, Float:collisions[][3])

Checks if a player is standing on any surface (prevent animation exploit when falling)
CA_IsPlayerOnSurface(playerid, Float:tolerance=1.5)

Removes all barries this function only works BEFORE using CA_Init()
CA_RemoveBarriers()

Removes all dynamic breakable objects, also only works BEFORE using CA_Init()
CA_RemoveBreakableBuildings()

Checks if a player is in the water depth returns the lowest collision point found playerdepth returns
how deep the player is below the surface
CA_IsPlayerInWater(playerid, &Float:depth, &Float:playerdepth)

Checks if a player is near water (returns 0 if they are actually in the water) distance is the radius to check
Height is how high above the water to detect
CA_IsPlayerNearWater(playerid, Float:dist=3.0, Float:height=3.0)

// Check if a player is blocked by a wall
CA_IsPlayerBlocked(playerid, Float:dist=1.5, Float:height=0.5)


Notes:

Recommended loading order
- RemoveBuildings
- Init static map
- Add static objects

There is currently no support for virtual worlds / interior or per player with streamer objects

*/
