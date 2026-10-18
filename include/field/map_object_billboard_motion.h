#ifndef POKEHEARTGOLD_FIELD_MAP_OBJECT_BILLBOARD_MOTION_H
#define POKEHEARTGOLD_FIELD_MAP_OBJECT_BILLBOARD_MOTION_H

#include "map_object.h"
#include "sprite.h"

Sprite *MapObject_GetBillboard(LocalMapObject *object);
void FollowMonDraw_UpdatePositionOnly(LocalMapObject *object);
void *MapObjectManager_GetFollowMonBillboard(MapObjectManager *manager);

void BillboardDraw_Init(void *object);
void BillboardDraw_Delete(void *object);
void BillboardDraw_Pause(void *object);
void BillboardDraw_Resume(void *object);
void FollowMonDraw_Init(void *object);
void FollowMonDraw_Delete(void *object);
void FollowMonDraw_Pause(void *object);
void FollowMonDraw_Resume(LocalMapObject *object);
void BillboardDraw_UpdateNpc(void *object);
void BillboardDraw_UpdatePositionOnly(void *object);
void BillboardDraw_UpdateLooping(void *object);
void FollowMonDraw_UpdateLarge(LocalMapObject *object);
void FollowMonDraw_Update(LocalMapObject *object);
void BillboardDraw_UpdatePcWoman(void *object);
void BillboardDraw_UpdateTwoSequences(void *object);
void PlayerDraw_Init(void *object);
void PlayerDraw_Delete(void *object);
void PlayerDraw_Pause(void *object);
void PlayerDraw_Resume(void *object);
void PlayerDraw_UpdateWalking(void *object);
void PlayerDraw_UpdateCycling(void *object);
void PlayerDraw_UpdateFishing(void *object);
void PlayerDraw_UpdateLadder(void *object);
void PlayerDraw_UpdateSurfing(void *object);
void PlayerDraw_UpdateUseHmOrSaving(void *object);
void PlayerDraw_UpdateWatering(void *object);
void PlayerDraw_UpdatePoketch(void *object);
void PlayerDraw_UpdateHeal(void *object);
void PlayerDraw_UpdateApricornShake(void *object);
void ModelObjectDraw_Init(void *object);
void ModelObjectDraw_Update(void);
void ModelObjectDraw_Delete(void *object);
void ModelObjectDraw_Pause(void *object);
void ModelObjectDraw_Resume(void *object);

#endif // POKEHEARTGOLD_FIELD_MAP_OBJECT_BILLBOARD_MOTION_H
