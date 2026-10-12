#ifndef POKEHEARTGOLD_FIELD_FIELD_FOLLOW_MON_H
#define POKEHEARTGOLD_FIELD_FIELD_FOLLOW_MON_H

#include "field_system.h"
#include "task.h"

void FollowMon_MarkForRecreate(FieldSystem *fieldSystem);
u32 FollowMon_GetFasterMovement(u32 direction, u32 movement);
fx32 FollowMon_GetHopHeight(LocalMapObject *object);
BOOL MapObject_IsShinyPokemon(LocalMapObject *object);
BOOL MapObject_HasFollowMonSprite(LocalMapObject *object);
u8 ov01_02205584(LocalMapObject *object);
u8 ov01_022055B0(LocalMapObject *object);
u8 FollowMonObject_IsLarge(LocalMapObject *object);
void MapObject_GetCoordsBehind(LocalMapObject *object, int *x, int *z);
void MapObject_GetPreviousCoordsBehind(LocalMapObject *object, int *x, int *z);
u8 MapObject_GetAdjacentTileBehavior(LocalMapObject *object, u32 direction);
void MapObject_SetPositionNextTo(LocalMapObject *anchor, LocalMapObject *object, int direction, int facing);
void FollowMon_SpawnReleaseBall(LocalMapObject *object);
void FollowMon_MoveToPlayer(FieldSystem *fieldSystem, u8 direction);
BOOL FollowMon_CheckVisibleFlag(FieldSystem *fieldSystem);
void FollowMon_SetNoBallEffect(FieldSystem *fieldSystem);
void ov01_022057DC(MapObjectManager *manager);
void FollowMonBillboard_SetResource(BOOL alternate, LocalMapObject *object, void *billboard);
void FollowMonBillboard_InitSequence(BOOL alternate, LocalMapObject *object, void *billboard, void *record);
void MapObject_SetBillboardSepia(void *unused, LocalMapObject *object, void *billboard);
void FollowMon_QueuePlayerStep(int movement, int x, int z, FollowMon *followMon);
void FollowMon_RecreateForMap(FieldSystem *fieldSystem);
void FollowMon_SetHeight(FieldSystem *fieldSystem, fx32 y);
BOOL Task_WaitFollowMonMovement(TaskManager *taskMan);
void FollowMon_StartHopIntoBall(FieldSystem *fieldSystem);
BOOL FollowMon_StartRecall(FieldSystem *fieldSystem);
void FollowMon_StartHideForFieldMove(TaskManager *taskMan);
void MapObject_StepTowards(LocalMapObject *target, LocalMapObject *object);
int SpriteId_GetModelNo(u32 spriteId);
void FollowMon_SetFacingDirection(FieldSystem *fieldSystem, u32 direction);
BOOL FollowMon_StartFlash(FieldSystem *fieldSystem, u8 step, u8 period);
BOOL FollowMon_IsNextToPlayer(FieldSystem *fieldSystem);
u8 FieldSystem_GetLeadMonIndex(FieldSystem *fieldSystem);

#endif // POKEHEARTGOLD_FIELD_FIELD_FOLLOW_MON_H
