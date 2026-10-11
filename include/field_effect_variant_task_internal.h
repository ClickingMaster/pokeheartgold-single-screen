#ifndef OV01_022006A8_PRIVATE_H
#define OV01_022006A8_PRIVATE_H

#include "global.h"

typedef struct Ov01Vec3 {
    s32 x;
    s32 y;
    s32 z;
} Ov01Vec3;

typedef struct Ov01Setup {
    u32 words[4];
} Ov01Setup;

typedef struct Ov01ObjectState {
    u32 unk00;
    s32 objectId;
    s32 mapId;
    u32 unk0C;
    Ov01Setup setup;
    void *effect;
} Ov01ObjectState;

typedef struct Ov01SpawnArgs {
    void *fieldSystem;
    void *context;
    void *renderer;
    void *mapObject;
} Ov01SpawnArgs;

extern void *FieldEffectManager_HeapAllocAndFill(u32, u32, u32, u32);
extern void FieldEffectManager_HeapFree(void *);
extern void *FieldEffectManager_GetFromMapObject(void *);
extern void *FieldEffectManager_GetRendererData(void *, u32);
extern void ov01_021F18D4(void *, u32, u32);
extern void FieldEffectManager_LoadTexResource(void *, u32, u32, u32);
extern void FieldEffectManager_AddRecord(void *, u32, u32, u32, u32, u32, const void *);
extern void ov01_021F18FC(void *, u32);
extern void FieldEffectManager_FreeTexResource(void *, u32);
extern void FieldEffectManager_ReleaseRecord(void *, u32);
extern void FieldEffectManager_CreateEffect(void *, const void *, const Ov01Vec3 *, u32, Ov01SpawnArgs *, u32);
extern void *MapObject_GetFieldSystem(void *);
extern s32 MapObject_GetPriorityPlusValue(void *, s32);
extern void *sub_02068D98(void *);
extern void sub_02068DA8(void *, Ov01Vec3 *);
extern s32 MapObject_GetID(void *);
extern s32 MapObject_GetMapID(void *);
extern void *FieldEffectManager_CreateBillboardFromRecord(void *, u32, const Ov01Vec3 *);
extern void BillboardObject_Delete(void *);
extern BOOL sub_0205F0A8(void *, s32, s32);
extern void FieldEffectManager_DestroyEffect(void *);
extern void MapObject_CopyPositionVector(void *, Ov01Vec3 *);
extern void MapObject_CopyFacingVector(void *, Ov01Vec3 *);
extern void BillboardObject_SetPosition(void *, const Ov01Vec3 *);
extern const u8 ov01_02209308[];
extern const u8 ov01_0220931C[];

void *ov01_022006A8(u32);
void ov01_022006C4(void *);
void ov01_022006D4(void *);
void ov01_02200710(void *);
void ov01_02200730(void *);
BOOL ov01_02200780(void *, Ov01ObjectState *);
void ov01_022007D0(void *, Ov01ObjectState *);
void ov01_022007DC(void *, Ov01ObjectState *);
void ov01_022007F8(void *, Ov01ObjectState *);

#endif
