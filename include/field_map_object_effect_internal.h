#ifndef PRIVATE_OV01_021FF464_H
#define PRIVATE_OV01_021FF464_H
#include "global.h"
typedef struct LocalCtx {
    void *owner;
} LocalCtx;
typedef struct LocalState {
    u32 stage, sprite, id, mapId, pad10, data14;
    void *resource;
    u32 pad1c;
    void *mapObject, *effect;
} LocalState;
void *FieldEffectManager_HeapAllocAndFill(void *, u32, u32, u32);
void FieldEffectManager_HeapFree(void *);
void ov01_021F18D4(void *, int, int);
void ov01_021F1908(void *, int, int);
void FieldEffectManager_LoadTexResource(void *, int, int, int);
void FieldEffectManager_AddRecord(void *, int, int, int, int, int, const void *);
void ov01_021F18FC(void *, int);
void ov01_021F1924(void *, int);
void FieldEffectManager_FreeTexResource(void *, int);
void FieldEffectManager_ReleaseRecord(void *, int);
void *FieldEffectManager_GetFromMapObject(void *);
void *FieldEffectManager_GetRendererData(void *, int);
void *MapObject_GetFieldSystem(void *);
int MapObject_GetPriorityPlusValue(void *, int);
void FieldEffectManager_CreateEffect(void *, const void *, const VecFx32 *, void *, void *, int);
void *FieldEffect_GetUserData(void *);
int FieldEffect_GetParam(void *);
void FieldEffect_SetPosition(void *, const VecFx32 *);
int MapObject_GetSpriteID(void *);
int MapObject_GetID(void *);
int MapObject_GetMapID(void *);
void *FieldEffectManager_CreateBillboardFromRecord(void *, int, const VecFx32 *);
BOOL MapObject_TestFlagsBits(void *, u32);
void BillboardObject_SetVisible(void *, BOOL);
void BillboardObject_Delete(void *);
BOOL sub_0205F0F8(void *, int, int, int);
BOOL MapObject_CheckFlag26(void *);
void FieldEffectManager_DestroyEffect(void *);
void BillboardObject_AdvanceFrame(void *, fx32);
fx32 BillboardObject_GetSequenceFrame(void *);
void BillboardObject_SetFrame(void *, fx32);
void MapObject_CopyPositionVector(void *, VecFx32 *);
void MapObject_CopyFacingVector(void *, VecFx32 *);
void BillboardObject_SetPosition(void *, const VecFx32 *);
LocalCtx *ov01_021FF464(void *);
void ov01_021FF480(LocalCtx *);
void ov01_021FF490(LocalCtx *);
void ov01_021FF4D4(LocalCtx *);
void ov01_021FF4FC(void *, void *);
BOOL ov01_021FF54C(void *, LocalState *);
void ov01_021FF5B8(void *, LocalState *);
void ov01_021FF5C4(void *, LocalState *);
void ov01_021FF658(void *, LocalState *);
#endif
