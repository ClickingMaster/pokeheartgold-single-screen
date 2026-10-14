#ifndef POKEHEARTGOLD_FIELD_DOOR_TRANSITION_H
#define POKEHEARTGOLD_FIELD_DOOR_TRANSITION_H

#include "field_system.h"
#include "task.h"

// Door transitions: the player walks into or out of a door prop on the map while its open/close animation plays
// and the camera zooms. The FieldTransitionWork handle is a DoorTransitionWork (door_transition.c).
FieldTransitionWork *DoorTransition_New(void);
void DoorTransition_Free(FieldTransitionWork *work);
void DoorTransition_SetPosition(u32 x, u32 z, FieldTransitionWork *work);
BOOL DoorTransition_WalkIntoDoor(FieldSystem *fieldSystem, FieldTransitionWork *work);
BOOL DoorTransition_WalkOutOfDoor(FieldSystem *fieldSystem, FieldTransitionWork *work);
void DoorTransition_StepCameraZoomIn(Camera *camera, u8 *step);
void DoorTransition_StepCameraZoomOut(Camera *camera, u8 *step);
int GetDoorSE(FieldSystem *fieldSystem, int modelId, BOOL opening);
int DoorTransition_GetCameraPathIndex(int modelId);
BOOL DoorTransition_WalkOutOfSideDoor(FieldSystem *fieldSystem, FieldTransitionWork *work, u8 direction);
BOOL DoorTransition_WalkIntoSideDoor(FieldSystem *fieldSystem, FieldTransitionWork *work, u8 direction);
BOOL Task_WaitDoorAnimation(TaskManager *taskman);
void *DoorTransition_NewTaskWork(void);
BOOL Task_WalkOutOfDoorAfterWarp(TaskManager *taskman);
BOOL Task_StairsFadeToWhite(TaskManager *taskman);
BOOL Task_StairsFadeOut(TaskManager *taskman);
BOOL Task_ArriveWithDirectionalFade(TaskManager *taskman);
BOOL Task_ArriveWithFade(TaskManager *taskman);
BOOL DoorTransition_ModelSkipsCameraZoom(FieldSystem *fieldSystem, int modelId);

#endif // POKEHEARTGOLD_FIELD_DOOR_TRANSITION_H
