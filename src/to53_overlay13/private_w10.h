#ifndef OVERLAY13_NETWORK_HELPERS_PRIVATE_H
#define OVERLAY13_NETWORK_HELPERS_PRIVATE_H

#include "global.h"

typedef struct Overlay13CipherState {
    u32 x;
    u32 y;
    u8 *table;
    u32 size;
} Overlay13CipherState;

extern void *ov13_0224DD80[];
extern void *ov13_0224DD8C[];
extern u8 ov13_0224DDA0[];

void MI_CpuCopy8(const void *, void *, u32);
void MI_CpuFill8(void *, u8, u32);
u8 ov13_022226C8(Overlay13CipherState *);
void *ov13_022228A4(s32, s32);
void *ov13_022228BC(s32, s32);
void ov13_02222968(void *, const void *, u32);
void ov13_02222978(void *, int, u32);
void ov13_022229FC(void);
s32 ov13_02222A9C(const char *);
void ov13_02222B20(void *);
void ov13_02222B2C(u32, void *);
void ov13_02222B34(const void *, void *);
void ov13_02222BE4(void *, const void *, u32);
BOOL ov13_02222BF4(void *);
BOOL ov13_02222C08(void *);
void ov13_022230F8(s32);
s32 ov13_02223100(s32);

#endif
