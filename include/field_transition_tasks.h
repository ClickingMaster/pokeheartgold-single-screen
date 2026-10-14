#ifndef POKEHEARTGOLD_FIELD_TRANSITION_TASKS_H
#define POKEHEARTGOLD_FIELD_TRANSITION_TASKS_H

#include "task.h"

typedef struct FieldTransitionUnkEnvironmentA {
    int state;
    int unk4;
} FieldTransitionUnkEnvironmentA;

// Map enter routines (unk_02055BF0.c sMapEnterRoutines, by FieldTransitionEnvironment.transitionNo)
BOOL FieldTransition_EnterThroughSideDoor(TaskManager *man);
BOOL FieldTransition_EnterFromWarpStairs(TaskManager *man);
BOOL FieldTransition_EnterRising(TaskManager *man);
BOOL FieldTransition_EnterWithDoorExit(TaskManager *man);
BOOL FieldTransition_EnterSinking(TaskManager *man);

// Player/camera placement before the enter routine (unk_02055BF0.c _020FC76C)
void FieldTransition_PlaceInsideSideDoor(FieldSystem *fieldSystem);
void FieldTransition_PlaceOffWarpStairs(FieldSystem *fieldSystem);
void FieldTransition_PlaceBelowFloor(FieldSystem *fieldSystem);
void FieldTransition_PlaceAboveFloor(FieldSystem *fieldSystem);

void CallTask_WiFiPlaza(TaskManager *taskManager, u16 parameter);

#endif // POKEHEARTGOLD_FIELD_TRANSITION_TASKS_H
