#ifndef TO40_UNK_020979A8_PRIVATE_H
#define TO40_UNK_020979A8_PRIVATE_H
#include "global.h"

#include "field_system.h"
#include "ride_cutscene_task.h"
#include "task.h"
typedef struct NightTaskEnv {
    u8 param, night;
    u8 pad02[2];
    void *destination;
} NightTaskEnv;
void LoadAreaOrDungeonLightTxt(u32 archiveId, ModelAttributes *attributes);
void CallTask_ShipVoyage(FieldSystem *, u8, u8, u32, u32, u32);
void ShipDepartureCutscene_LaunchApp(FieldSystem *, void *);
void CallTask_LeaveOverworld(TaskManager *);
void CallTask_RestoreOverworldAndFadeIn(TaskManager *);
#endif
