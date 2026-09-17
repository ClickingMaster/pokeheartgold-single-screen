#ifndef PRIVATE_H
#define PRIVATE_H
#include "global.h"

#include "bg_window.h"
typedef struct SigInput {
    u8 xs[8];
    u8 ys[8];
    u8 style : 4;
    u8 count : 4;
} SigInput;
typedef struct SigState {
    u8 x;
    u8 y;
    u16 count;
} SigState;
extern const u8 ov52_021E95A0[][16];
void ov52_021E8F2C(Window *, const void *, s32, s32, s32, s32, s32, s32, s32, s32);
void ov52_021E8FAC(Window *, const void *, s32, s32, s32 *, s32 *, s32, s32);
void ov52_021E9124(const SigInput *, SigState *);
void ov52_021E9158(Window *, SigInput *, SigState *, u32);
#endif
