#ifndef TO37_OV49_R70_PRIVATE_H
#define TO37_OV49_R70_PRIVATE_H

#include "global.h"

#define U8_AT(p, n)  (*(u8 *)((u8 *)(p) + (n)))
#define U32_AT(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define PTR_AT(p, n) (*(void **)((u8 *)(p) + (n)))

#pragma require_prototypes off

typedef struct Ov49AnimationState {
    u8 pad_000[0x87C];
    const u8 *descs[18];
    s32 frames[18][2];
} Ov49AnimationState;

void GF_AssertFail(void);
void ModelAnimation_SetFrame(void *, s32);
s32 ModelAnimation_GetFrame(void *);
s32 ModelAnimation_GetFrameCount(void *);
void ModelInstance_Init(void *, void *);
void ModelInstance_AddAnimation(void *, void *);
void ModelInstance_RemoveAnimation(void *, void *);
void ModelInstance_Draw(void *);
BOOL ModelInstance_IsVisible(void *);

void ov49_02265B3C(void *, Ov49AnimationState *, u32, u32, s32);
BOOL ov49_02265B94(void *, Ov49AnimationState *, u32, u32, s32);

#endif
