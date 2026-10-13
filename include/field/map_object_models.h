#ifndef POKEHEARTGOLD_FIELD_MAP_OBJECT_MODELS_H
#define POKEHEARTGOLD_FIELD_MAP_OBJECT_MODELS_H

#include "field_system.h"
#include "map_object.h"
#include "sprite.h"

// Map object models: the map model NARC and the billboard renderer's lifetime, the graphics info of each sprite id,
// and the billboard callbacks of the following Pokemon, the Ho-Oh / Lugia objects and the camera focus object.

void MapObjectManager_InitRender(MapObjectManager *mapObjectManager, int textureCount, int objectsToPreload, const int *objectIDs, int loadsPerFrame);
void MapObjectManager_FreeRender(MapObjectManager *mapObjectManager);
void FldObjSys_OpenMModelNarc(MapObjectManager *manager);
void FldObjSys_CloseMModelNarc(MapObjectManager *manager);
void *ReadMModelFromNarcInternal(MapObjectManager *manager, u32 memberIdx, BOOL atStart);

ObjectEventGraphicsInfo *ObjectEvent_GetGraphicsInfo(u32 spriteId);
int GetMoveModelNoBySpriteId(u32 spriteId);
const u16 *MapObject_GetEffectFlags(LocalMapObject *object);
const u16 *SpriteId_GetEffectFlags(u32 spriteId);

BOOL MapObject_IsBillboardAnimPaused(LocalMapObject *object);
// Where the object's billboard is drawn: its position plus its facing vector and the two sprite offset vectors.
void MapObject_GetDrawPosition(LocalMapObject *object, VecFx32 *dest);
void MapObject_TryUpdateDraw(LocalMapObject *object);
void MapObject_SetFacingAndUpdateDraw(LocalMapObject *object, u32 direction);

void FollowMonBillboard_Update(LocalMapObject *object, Sprite *billboard, void *work, u32 direction, BOOL addFacingOffset);
void FollowMonBillboard_AddFacingOffset(LocalMapObject *object, u32 direction, VecFx32 *offset);
void FollowMonBillboard_SetShadowAnchored(LocalMapObject *object, BOOL anchored);
void FollowMonBillboard_SetHeight(LocalMapObject *object, s8 height);
void FollowMonBillboard_AddHeight(LocalMapObject *object, s8 delta);
BOOL FollowMonBillboard_IsShadowFollowing(LocalMapObject *object);
void FollowMonBillboard_GetShadowOffset(LocalMapObject *object, VecFx32 *offset);
void FollowMonBillboard_LoadSequence(BOOL alternate, LocalMapObject *object);
void FollowMonBillboard_ResetPalette(LocalMapObject *object);

void LegendaryBirdObject_Init(LocalMapObject *object);
void LegendaryBirdObject_Update(LocalMapObject *object);
void LegendaryBirdObject_Delete(LocalMapObject *object);
void LegendaryBirdObject_Pause(LocalMapObject *object);
void LegendaryBirdObject_Resume(LocalMapObject *object);

void CameraFocusObject_Init(LocalMapObject *object);
void CameraFocusObject_Update(LocalMapObject *object);
void CameraFocusObject_Delete(LocalMapObject *object);
void CameraFocusObject_Pause(LocalMapObject *object);
void CameraFocusObject_Resume(LocalMapObject *object);

#endif // POKEHEARTGOLD_FIELD_MAP_OBJECT_MODELS_H
