#ifndef POKEHEARTGOLD_TOUCH_SAVE_APP_H
#define POKEHEARTGOLD_TOUCH_SAVE_APP_H

#include "task.h"

SysTask *TouchSaveApp_Init(BgConfig *bgConfig, void *a1, FieldSystem *fieldSystem, void *a3);
void TouchSaveApp_Exit(BgConfig *bgConfig, SysTask *task);
BOOL TouchSaveApp_IsExitDone(void *a0);

#endif
