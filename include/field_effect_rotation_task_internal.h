#ifndef OV01_021FEC38_PRIVATE_H
#define OV01_021FEC38_PRIVATE_H

#include "global.h"

typedef struct Ov01Vec3 {
    s32 x;
    s32 y;
    s32 z;
} Ov01Vec3;

typedef struct Ov01EffectManager {
    void *context;
    u8 primary[0x14];
    u8 secondary[0x24];
} Ov01EffectManager;

typedef struct Ov01SpawnArgs {
    void *context;
    void *renderer;
} Ov01SpawnArgs;

typedef struct Ov01TaskArgs {
    void *context;
    void *resource;
} Ov01TaskArgs;

typedef struct Ov01EffectState {
    u8 unk00[0x78];
    Ov01TaskArgs args;
} Ov01EffectState;

extern void *FieldEffectManager_HeapAllocAndFill(u32, u32, u32, u32);
extern void FieldEffectManager_HeapFree(void *);
extern void ov01_021F19F4(void *, void *, u32, u32, u32);
extern void ov01_021F1A18(void *, void *, u32, u32, u32);
extern void RenderModel_Free(void *);
extern void RenderAnimation_Free(void *);
extern BOOL MapObject_TestFlagsBits(void *, u32);
extern void *FieldEffectManager_GetFromMapObject(void *);
extern void *FieldEffectManager_GetRendererData(void *, u32);
extern void sub_020611C8(void *, u32, Ov01Vec3 *);
extern s32 MapObject_GetPreviousYCoord(void *);
extern s32 MapObject_GetPriorityPlusValue(void *, s32);
extern void FieldEffectManager_CreateEffect(void *, const void *, const Ov01Vec3 *, u32, Ov01SpawnArgs *, s32);
extern Ov01TaskArgs *sub_02068D98(void *);
extern void ov01_021F1A34(void *, void *, void *, void *, u32);
extern void RenderObj_InitWithAnimation(void *, void *, void *);
extern BOOL RenderAnimation_IsDone(void *);
extern void FieldEffectManager_DestroyEffect(void *);
extern void RenderAnimation_Advance(void *, s32, u32);
extern void sub_02068DB8(void *, Ov01Vec3 *);
extern void RenderObj_DrawAt(void *, const Ov01Vec3 *);
extern const u8 ov01_02209110[];

void *ov01_021FEC38(u32);
void ov01_021FEC54(void *);
void ov01_021FEC64(Ov01EffectManager *);
void ov01_021FEC8C(Ov01EffectManager *);
void ov01_021FECA0(void *, void *, u32, u32);
BOOL ov01_021FED14(void *, Ov01EffectState *);
void ov01_021FED4C(void *, void *);
void ov01_021FED58(void *, void *);
void ov01_021FED80(void *, void *);

#endif
