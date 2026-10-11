#ifndef POKEHEARTGOLD_TOUCH_MART_APP_H
#define POKEHEARTGOLD_TOUCH_MART_APP_H

#include "bg_window.h"
#include "field_system.h"
#include "overlay_03.h"
#include "sys_task.h"

SysTask *TouchMartApp_Init(BgConfig *bgConfig, void *a1, FieldSystem *fieldSystem, MartData *martData);
void TouchMartApp_Exit(BgConfig *bgConfig, SysTask *task);
BOOL TouchMartApp_IsExitDone(void);

#endif // POKEHEARTGOLD_TOUCH_MART_APP_H
