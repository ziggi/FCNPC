/* =========================================
			
		FCNPC - Fully Controllable NPC
			----------------------

	- File: Natives.h
	- Author(s): OrMisicL

  =========================================*/

#ifndef NATIVES_H
#define NATIVES_H

class CNatives
{
	public:
		static cell AMX_NATIVE_CALL FCNPC_SetUpdateRate(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_InitZMap(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_ProcessDamage(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_Create(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_Destroy(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_Spawn(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_Respawn(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_IsSpawned(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_Kill(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_IsDead(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_SetPosition(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetPosition(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_SetQuaternion(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetQuaternion(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_SetAngle(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetAngle(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_SetVelocity(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetVelocity(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_SetHealth(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetHealth(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_SetArmour(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetArmour(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_SetSkin(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetSkin(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_SetInterior(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetInterior(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_SetWeapon(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetWeapon(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_SetAmmo(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetAmmo(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_SetKeys(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetKeys(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_SetSpecialAction(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetSpecialAction(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_GoTo(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_Stop(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_IsMoving(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_AimAt(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_StopAim(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_MeleeAttack(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_StopAttack(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_IsAiming(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_IsShooting(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_IsReloading(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_EnterVehicle(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_ExitVehicle(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_PutInVehicle(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_RemoveFromVehicle(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetVehicleID(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetVehicleSeat(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_StartRecordingPlayback(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_StopRecordingPlayback(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_PauseRecordingPlayback(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_ResumeRecordingPlayback(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_OpenNode(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_CloseNode(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_IsNodeOpen(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetNodeType(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetNodePointPosition(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_SetNodePoint(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetNodePointCount(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_GetNodeInfo(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_PlayNode(AMX *amx, cell *params);
		static cell AMX_NATIVE_CALL FCNPC_StopPlayingNode(AMX *amx, cell *params);

		static cell AMX_NATIVE_CALL FCNPC_GetZGround(AMX *amx, cell *params);
};

#endif