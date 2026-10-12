#ifndef POKEHEARTGOLD_FIELD_DOOR_TRANSITION_H
#define POKEHEARTGOLD_FIELD_DOOR_TRANSITION_H

#include "field_system.h"
#include "task.h"

// Door transitions: the player walks into or out of a door prop on the map while its open/close animation plays
// and the camera zooms. The FieldEnvSubUnk18 handle is a DoorTransitionWork (door_transition.c).
FieldEnvSubUnk18 *DoorTransition_New(void);
void DoorTransition_Free(FieldEnvSubUnk18 *work);
void DoorTransition_SetPosition(u32 x, u32 z, FieldEnvSubUnk18 *work);
BOOL DoorTransition_WalkIntoDoor(FieldSystem *fieldSystem, FieldEnvSubUnk18 *work);
BOOL DoorTransition_WalkOutOfDoor(FieldSystem *fieldSystem, FieldEnvSubUnk18 *work);
void DoorTransition_StepCameraZoomIn(Camera *camera, u8 *step);
void DoorTransition_StepCameraZoomOut(Camera *camera, u8 *step);
int GetDoorSE(FieldSystem *fieldSystem, int modelId, BOOL opening);
int DoorTransition_GetCameraPathIndex(int modelId);
BOOL DoorTransition_WalkOutOfSideDoor(FieldSystem *fieldSystem, FieldEnvSubUnk18 *work, u8 direction);
BOOL DoorTransition_WalkIntoSideDoor(FieldSystem *fieldSystem, FieldEnvSubUnk18 *work, u8 direction);
BOOL Task_WaitDoorAnimation(TaskManager *taskman);
void *DoorTransition_NewTaskWork(void);
BOOL Task_WalkOutOfDoorAfterWarp(TaskManager *taskman);
BOOL Task_StairsFadeToWhite(TaskManager *taskman);
BOOL Task_StairsFadeOut(TaskManager *taskman);
BOOL Task_ArriveWithDirectionalFade(TaskManager *taskman);
BOOL Task_ArriveWithFade(TaskManager *taskman);
BOOL DoorTransition_ModelSkipsCameraZoom(FieldSystem *fieldSystem, int modelId);

#endif // POKEHEARTGOLD_FIELD_DOOR_TRANSITION_H
