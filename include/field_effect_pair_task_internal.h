#ifndef PRIVATE_OV01_02200040_H
#define PRIVATE_OV01_02200040_H
#include "global.h"
typedef struct LocalManager {
    void *owner;
} LocalManager;
typedef struct LocalState {
    u32 stage;
    u32 objectId;
    u32 mapId;
    u32 pad0C;
    u32 args0;
    void *resource;
    u32 args8;
    void *mapObject;
    void *effect;
} LocalState;
typedef struct LocalFourWords {
    u32 v[4];
} LocalFourWords;
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
void MapObject_CopyPositionVector(void *, VecFx32 *);
int MapObject_GetXCoord(void *);
int MapObject_GetZCoord(void *);
void sub_020611C8(int, int, VecFx32 *);
int MapObject_GetPriorityPlusValue(void *, int);
void FieldEffectManager_CreateEffect(void *, const void *, const VecFx32 *, void *, const void *, int);
void *FieldEffect_GetUserData(void *);
void FieldEffect_GetPosition(void *, VecFx32 *);
int MapObject_GetID(void *);
int MapObject_GetMapID(void *);
void *FieldEffectManager_CreateBillboardFromRecord(void *, int, const VecFx32 *);
void BillboardObject_Delete(void *);
void BillboardObject_AdvanceFrame(void *, fx32);
fx32 BillboardObject_GetSequenceFrame(void *);
void FieldEffectManager_DestroyEffect(void *);
void BillboardObject_SetPosition(void *, const VecFx32 *);
LocalManager *ov01_02200040(void *);
void ov01_0220005C(LocalManager *);
void ov01_0220006C(LocalManager *);
void ov01_022000B4(LocalManager *);
void ov01_022000DC(void *);
BOOL ov01_02200140(void *, LocalState *);
void ov01_02200184(void *, LocalState *);
void ov01_02200190(void *, LocalState *);
void ov01_022001C0(void *, LocalState *);
#endif
