#ifndef TO51_R4_W14_PRIVATE_H
#define TO51_R4_W14_PRIVATE_H
#include "global.h"
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S16AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U8AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define PAT(p, o)   (*(void **)((u8 *)(p) + (o)))
void ManagedSprite_GetPositionXYWithSubscreenOffset(void *, s16 *, s16 *, fx32);
void ManagedSprite_SetPositionXYWithSubscreenOffset(void *, s16, s16, fx32);
BOOL ManagedSprite_IsAnimated(void *);
void ManagedSprite_SetAnim(void *, u32);
s32 ov93_0225FEAC(void *);
u32 ov93_02260B70(s32);
void ov93_02260A8C(void *);
void ov93_02260AD8(void *, void *);
void PlaySE(u16);
#endif
