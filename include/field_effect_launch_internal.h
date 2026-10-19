#ifndef POKEHEARTGOLD_FIELD_EFFECT_LAUNCH_INTERNAL_H
#define POKEHEARTGOLD_FIELD_EFFECT_LAUNCH_INTERNAL_H

#include "global.h"

#include "field_system.h"
#include "ride_cutscene_task.h"
#include "task.h"

typedef struct GenderTaskEnv {
    u8 variant;
    u8 gender;
    u8 night;
    u8 pad03;
    void *lighting;
} GenderTaskEnv;

void LoadAreaOrDungeonLightTxt(u32 archiveId, ModelAttributes *attributes);
void CallTask_MagnetTrainRide(FieldSystem *fieldSystem, u8 variant, u8 direction, u32 mapId, u32 x, u32 y);
void MagnetTrainCutscene_LaunchApp(FieldSystem *fieldSystem, void *args);
void CallTask_LeaveOverworld(TaskManager *taskManager);
void CallTask_RestoreOverworldAndFadeIn(TaskManager *taskManager);
void CallTask_MagnetTrainCutscene(FieldSystem *fieldSystem, u8 variant, void *lighting);
BOOL Task_MagnetTrainCutscene(TaskManager *taskManager);
BOOL Task_MagnetTrainRide(TaskManager *taskManager);

#endif
