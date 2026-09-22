#ifndef TO57_OVERLAY91_058_PRIVATE_H
#define TO57_OVERLAY91_058_PRIVATE_H

#include "global.h"

#include "math_util.h"

typedef struct T57Anim {
    u8 raw[0x78];
} T57Anim;

typedef struct T57Slot {
    u8 raw[0x14];
} T57Slot;

typedef struct T57Work {
    T57Anim anim[3];
    u8 unk168[0x30];
    T57Slot slots[6];
    volatile u16 current;
    u16 state;
} T57Work;

void sub_02018198(void *, u32);
void sub_020181D4(void *, void *);
void sub_020181E0(void *, void *);
void sub_020182A0(void *, int);
void sub_020182E0(void *, u16, int);
u32 sub_0201815C(void *, u32);
void GF_AssertFail(void);
u32 _u32_div_f(u32, u32);
s32 _s32_div_f(s32, s32);
extern const u8 ov91_0226274C[];

void ov91_0226014C(u8 *, u8 *, int);
void ov91_022601C8(u8 *, u8 *);
void ov91_022601F4(u8 *, u8 *);
void ov91_02260218(u8 *, u8 *);
void ov91_02260254(u8 *, u16);
void ov91_02260298(u8 *, u32);
u32 ov91_0226031C(u8 *, u32);
void ov91_02260334(u8 *, int);

#endif
