#ifndef TO50_PALETTE_FADE_12090_PRIVATE_H
#define TO50_PALETTE_FADE_12090_PRIVATE_H
#include "global.h"

#include "heap.h"
#include "sys_task_api.h"
typedef struct EffectWork12090 {
    u32 f[12];
} EffectWork12090;
typedef struct EffectConfig12090 {
    const void *src0;
    const void *src1;
    u16 count;
    u16 mode;
    u8 a;
    u8 b;
    u16 value;
} EffectConfig12090;
void sub_02010AB0(void *, void *, void *, const void *, const void *, u32);
void sub_02010E64(void *, u16, u32, u32);
void sub_02012204(void *);
void sub_02010F00(SysTask *, void *);
s16 *sub_02010EE0(void *, int);
void sub_02010F84(void *, u8, u8, u16, u32, s32, s32, s32, s32, u32);
void sub_02011068(void *, u32, u32, u32);
void sub_02010C38(void);
void sub_0200FF88(void *, void *, void (*)(void), u32, u32);
void sub_02012090(EffectWork12090 *, const EffectConfig12090 *, u32, u32, u32, u32, u32, u32);
#endif
