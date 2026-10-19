#ifndef POKEHEARTGOLD_AREA_LIGHT_TASKS_INTERNAL_H
#define POKEHEARTGOLD_AREA_LIGHT_TASKS_INTERNAL_H
#include "global.h"

#include "field_system.h"
#include "ride_cutscene_task.h"
#include "task.h"
typedef struct NightTaskEnv {
    u8 port, night;
    u8 pad02[2];
    void *modelAttributes;
} NightTaskEnv;
void LoadAreaOrDungeonLightTxt(u32 archiveId, ModelAttributes *attributes);
void CallTask_ShipVoyage(FieldSystem *, u8, u8, u32, u32, u32);
void ShipDepartureCutscene_LaunchApp(FieldSystem *, void *);
void CallTask_LeaveOverworld(TaskManager *);
void CallTask_RestoreOverworldAndFadeIn(TaskManager *);
#endif
