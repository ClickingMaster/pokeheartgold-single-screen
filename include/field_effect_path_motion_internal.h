#include "global.h"

typedef struct FourWordsPrivate {
    u32 words[4];
} FourWordsPrivate;
typedef struct VecFx32Private {
    s32 x, y, z;
} VecFx32Private;
typedef struct EffectContextPrivate {
    void *fieldSystem, *manager, *work, *mapObject;
} EffectContextPrivate;

extern void *FieldEffectManager_HeapAllocAndFill(void *, u32, u32, u32);
extern void FieldEffectManager_HeapFree(void *);
extern void ov01_021F18D4(void *, u32, u32);
extern void ov01_021F1908(void *, u32, u32);
extern void FieldEffectManager_LoadTexResource(void *, u32, u32, u32);
extern void FieldEffectManager_AddRecord(void *, u32, u32, u32, u32, u32, const void *);
extern void ov01_021F18FC(void *, u32);
extern void ov01_021F1924(void *, u32);
extern void FieldEffectManager_FreeTexResource(void *, u32);
extern void FieldEffectManager_ReleaseRecord(void *, u32);
extern const u32 ov01_02209200[];
extern const u32 ov01_022091EC[];
extern void *FieldEffect_GetUserData(void *);
extern void FieldEffect_GetPosition(void *, VecFx32Private *);
extern u32 MapObject_GetID(void *);
extern u32 MapObject_GetMapID(void *);
extern void *FieldEffectManager_CreateBillboardFromRecord(void *, u32, const VecFx32Private *);
extern void BillboardObject_Delete(void *);
extern void BillboardObject_AdvanceFrame(void *, s32);
extern s32 BillboardObject_GetSequenceFrame(void *);
extern void FieldEffectManager_DestroyEffect(void *);
extern void BillboardObject_SetPosition(void *, const VecFx32Private *);
extern void *FieldEffectManager_GetFromMapObject(void *);
extern void *MapObject_GetFieldSystem(void *);
extern void *FieldEffectManager_GetRendererData(void *, u32);
extern void MapObject_CopyPositionVector(void *, VecFx32Private *);
extern s32 MapObject_GetXCoord(void *);
extern s32 MapObject_GetZCoord(void *);
extern void sub_020611C8(s32, s32, VecFx32Private *);
extern u32 MapObject_GetPriorityPlusValue(void *, u32);
extern void FieldEffectManager_CreateEffect(void *, const void *, const VecFx32Private *, u32, const EffectContextPrivate *, u32);

void *ov01_021FF6B0(void *);
void ov01_021FF6CC(void *);
void ov01_021FF6DC(void *);
void ov01_021FF724(void *);
void ov01_021FF74C(void *);
BOOL ov01_021FF7B0(void *, void *);
void ov01_021FF7F4(void *, void *);
void ov01_021FF800(void *, void *);
void ov01_021FF830(void *, void *);
