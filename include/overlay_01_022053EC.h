#ifndef POKEHEARTGOLD_OVERLAY_01_022053EC_H
#define POKEHEARTGOLD_OVERLAY_01_022053EC_H

#include "field_system.h"

u32 ShayminFormChange_Spawn(FieldEffectManager *fieldEffectManager, VecFx32 *a1);
BOOL ShayminFormChange_IsDone();
void ov01_02205424(FieldSystem *fieldSystem);
BOOL ov01_0220553C(LocalMapObject *mapObject);
void ov01_022057D0(FieldSystem *fieldSystem);
void ov01_022057DC(MapObjectManager *mapObjectManager);
void ov01_02205990(int a0, int x, int z, FollowMon *mon);
void ov01_022059AC(FieldSystem *fieldSystem);
void ov01_02205A34(FieldSystem *fieldSystem, fx32 y);
BOOL ov01_02205A60(TaskManager *taskMan);

#endif // POKEHEARTGOLD_OVERLAY_01_022053EC_H
