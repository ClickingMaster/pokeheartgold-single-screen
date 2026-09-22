#ifndef TO30_OV74_PRIVATE_H
#define TO30_OV74_PRIVATE_H

#include "global.h"

#define U8AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PAT(p, o)   (*(void **)((u8 *)(p) + (o)))

typedef struct Ov74WmState {
    u8 state;
    u8 substate;
    u8 mode;
    u8 field3;
    u32 field4;
    u8 pad08[4];
    u16 field0c;
    u8 pad0e[6];
    u32 field14;
    u32 field18;
    u8 pad1c[4];
    u32 field20;
    u32 field24;
    u32 field28;
    u32 field2c;
    u32 field30;
    u8 pad34[4];
    u32 field38;
    u32 field3c;
    u32 field40;
    u32 field44;
} Ov74WmState;

typedef struct New33Task {
    u8 pad[8];
    s32 result;
} New33Task;

#endif
