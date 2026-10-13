#ifndef POKEHEARTGOLD_OVERLAY_02_EVENT_CUTSCENE_H
#define POKEHEARTGOLD_OVERLAY_02_EVENT_CUTSCENE_H

#include "field_system.h"

BOOL FieldSystem_FollowMonShayminNeedsRevert(FieldSystem *fieldSystem);
void StartShayminRevertToLandForm(TaskManager *taskMan);
void FieldSystem_BeginCelebiTimeTravelCutsceneTask(FieldSystem *fieldSystem);
void FieldSystem_BeginSinjohCutsceneTask(FieldSystem *fieldSystem);
void FieldSystem_BeginSinjohGetEggCutsceneTask(FieldSystem *fieldSystem, u8 a1);

#endif
