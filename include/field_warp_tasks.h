#ifndef POKEHEARTGOLD_FIELD_WARP_TASKS_H
#define POKEHEARTGOLD_FIELD_WARP_TASKS_H

#include "task.h"

void FieldMapChange_Set3DDisplay(FieldSystem *fieldSystem);
void FieldSystem_InitMapOnEnter(FieldSystem *fieldSystem, BOOL isConnection);
TaskManager *CallFieldTask_NewGame(FieldSystem *fieldSystem);
TaskManager *CallFieldTask_ContinueGame_Normal(FieldSystem *fieldSystem);
TaskManager *CallFieldTask_ContinueGame_CommError(FieldSystem *fieldSystem);
void CallTask_ChangeMap(TaskManager *taskManager, const Location *location);
void CallTask_ChangeMapToCoords(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction);
void CallTask_ScriptWarp(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction);
TaskManager *CallFieldTask_FlyWarp(FieldSystem *fieldSystem, u32 mapId, int warpId, int x, int y, int direction);
void JumpTask_FlyWarp(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction);
void JumpTask_FieldMoveWarp(TaskManager *taskManager, Location *location, int a2);
TaskManager *CallFieldTask_WarpPanel(FieldSystem *fieldSystem, u32 mapId, int warpId);
void CallFieldTask_ExitUnionRoom(FieldSystem *fieldSystem);
void CallTask_EnterUnionRoom(TaskManager *taskManager);
void CallTask_WarpToColosseum(TaskManager *taskManager, u32 mapId, int warpId, int x, int y, int direction);
void CallTask_ReturnFromColosseum(TaskManager *taskManager);

#endif // POKEHEARTGOLD_FIELD_WARP_TASKS_H
